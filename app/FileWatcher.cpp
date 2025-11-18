#include "FileWatcher.hpp"
#include "StringUtils.hpp"

#include <poll.h>           // poll()
#include <unistd.h>         // close()
#include <sys/inotify.h>    // inotify_add_watch(), inotify_rm_watch()

#include <array>            // std::array<>{}
#include <cerrno>           // errno
#include <vector>           // std::vector<>{}
#include <iomanip>          // std::quoted<>{}
#include <ostream>          // std::endl(), operator<<()
#include <iostream>          // std::cout (DEBUG only)
#include <algorithm>        // std::find_if<>()
#include <stdexcept>        // std::logic_error{}
#include <string_view>      // std::string_view{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

std::string inotifyEventMaskToString( int const event_mask )
{
    static awo::StringMappings const mappings
    {
#define MAPPING( x ) { x, #x },
        MAPPING( IN_ACCESS )
        MAPPING( IN_MODIFY )
        MAPPING( IN_ATTRIB )

        MAPPING( IN_CLOSE )
        MAPPING( IN_CLOSE_WRITE )
        MAPPING( IN_CLOSE_NOWRITE )

        MAPPING( IN_OPEN )

        MAPPING( IN_MOVE )
        MAPPING( IN_MOVED_FROM )
        MAPPING( IN_MOVED_TO )

        MAPPING( IN_CREATE )
        MAPPING( IN_DELETE )

        MAPPING( IN_DELETE_SELF )
        MAPPING( IN_MOVE_SELF )

        MAPPING( IN_UNMOUNT )
        MAPPING( IN_Q_OVERFLOW )
        MAPPING( IN_IGNORED )
#undef  MAPPING
    };

    return awo::bitmaskToString( event_mask, mappings );
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

FileWatcher::FileWatcher( std::ostream& logger )
: ThreadLoop( logger )
, inotify_fd{ inotify_init() }
{
    logger << "FileWatcher ctor entered..." << std::endl;

    checkForPosixError( inotify_fd, "inotify_init()" );

    registerActionForFD( inotify_fd, [ this ]{ handleINotification(); } );

    logger << "FileWatcher ctor done." << std::endl;
}

FileWatcher::~FileWatcher()
{
    logger << "FileWatcher dtor entered..." << std::endl;

    for ( auto const& [ filename, wdaction ]: file_watchers )
    {
        auto const& [ wd, action ] = wdaction;

        logger << "Removing file watcher for " << std::quoted( filename ) << " with wd = " << wd << std::endl;
        inotify_rm_watch( inotify_fd, wd );
    }

    logger << "Closing inotify_fd = " << inotify_fd << " ... ";
    auto const close_result = close( inotify_fd );
    checkForPosixError( close_result, "close()" );

    logger << "FileWatcher dtor done." << std::endl;
}

void FileWatcher::watchFile( std::string const& filename, Action const& action )
{
    std::lock_guard const locker( file_watchers_mutex );

    if ( !file_watchers.contains( filename ) )
    {
        logger << "Calling inotify_add_watch"           << "( "
                "fd = " << inotify_fd                   << ", "
                "name = " << std::quoted( filename )    << ", "
                "mask = IN_MODIFY"                      << " )"
                << std::endl;

        auto const wd = inotify_add_watch( inotify_fd, filename.c_str(), IN_MODIFY );
        checkForPosixError( wd, "inotify_add_watch()" );

        file_watchers[ filename ] = { wd, action };
    }
}

void FileWatcher::ignoreFile( std::string const& filename )
{
    std::lock_guard const locker( file_watchers_mutex );

    auto const p = file_watchers.find( filename );

    if ( p != file_watchers.end() )
    {
        auto const& [ wd, action ] = p->second;

        logger << "Calling inotify_rm_watch"    << "( "
                "fd = " << inotify_fd           << ", "
                "wd = " << wd                   << " )"
                << std::endl;

        auto const result = inotify_rm_watch( inotify_fd, wd );
        checkForPosixError( result, "inotify_rm_watch()" );

        file_watchers.erase( filename );
    }
}

bool FileWatcher::isWatching( std::string const& filename )
{
    std::lock_guard const locker( file_watchers_mutex );

    auto const p = file_watchers.find( filename );

    return p != file_watchers.end();
}

void FileWatcher::handleINotification()
{
    // Buffer, large enough to hold an inotify_event structure and a filename
    std::array< char, sizeof ( inotify_event ) + NAME_MAX > buffer;

    // Read the inotify event structure into the raw-bytes buffer
    auto const read_result = read( inotify_fd, buffer.data(), buffer.size() );
    checkForPosixError( read_result, "read()" );

    // Treat the buffer contents as an inotify event structure (with optional filename)
    auto const& inotifyEvent = *reinterpret_cast< inotify_event const* >( buffer.data() );
    logger << "inotifyEvent.wd     = " << inotifyEvent.wd << std::endl;
    logger << "inotifyEvent.mask   = " << inotifyEventMaskToString( inotifyEvent.mask ) << std::endl;
    logger << "inotifyEvent.cookie = " << inotifyEvent.cookie << std::endl;
    logger << "inotifyEvent.len    = " << inotifyEvent.len << std::endl;

    if ( inotifyEvent.len > 0 ) // ignore empty filenames
    {
        std::string_view const filename( inotifyEvent.name, inotifyEvent.len );

        logger << "inotifyEvent.name   = " << std::quoted( filename ) << std::endl;
    }

    // Lookup which action to execute for this wd and execute it
    executeActionForWD( inotifyEvent.wd );
}

void FileWatcher::executeActionForWD( int const event_wd )
{
    std::lock_guard const locker( file_watchers_mutex );

    // Find the entry (in the file_watchers map) corrensponding to this wd.

    auto const p = std::ranges::find_if( file_watchers, [ & ]( auto const& map_entry )
    {
        auto const& [ filename, wdaction ] = map_entry;
        auto const& [ wd, action ] = wdaction;

        return wd == event_wd;
    } );

    // If we found it, extract and execute the registered action for this wd's file.

    if ( p != file_watchers.end() )
    {
        auto const& [ filename, wdaction ] = *p;
        auto const& [ wd, action ] = wdaction;

        std::cout << "wd " << wd << " corresponds to file " << std::quoted( filename ) << std::endl;

        std::invoke( action );
    }
}
