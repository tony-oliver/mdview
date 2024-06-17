#include "FileWatcher.hpp"

#include <sys/inotify.h>

#include <unistd.h>         // pipe(), close()
#include <poll.h>           // poll_fd{}, poll()

#include <array>            // std::array<>{}
#include <cerrno>           // errno
#include <vector>           // std::vector<>{}
#include <iomanip>          // std::quoted<>{}
#include <ostream>          // std::endl, operator<<()
#include <iostream>         // std::clog{}
#include <iterator>         // std::data<>(), std::size<>()
#include <algorithm>        // std::find_if<>()
#include <stdexcept>        // std::logic_error{}
#include <string_view>      // std::string_view{}
#include <system_error>     // std::system_error{}

FileWatcher::FileWatcher()
: inotify_fd{ inotify_init() }
{
    if ( inotify_fd == -1 )
    {
        throw std::system_error( errno, std::generic_category(), "inotify_init()" );
    }

    registerActionForFD( inotify_fd, [ this ]{ handleINotification(); } );
}

FileWatcher::~FileWatcher()
{
    for ( auto const& [ filename, wdaction ]: file_watchers )
    {
        auto const& [ wd, action ] = wdaction;

        std::clog << "Removing file watcher with wd = " << wd << std::endl;
        inotify_rm_watch( inotify_fd, wd );
    }

    std::clog << "Closing inotify fd " << inotify_fd << std::endl;
    close( inotify_fd );
}

void FileWatcher::watchFile( std::string const& filename, Action const& action )
{
    std::lock_guard const locker( file_watchers_mutex );

    if ( !file_watchers.contains( filename ) )
    {
        constexpr int flags = IN_MODIFY | IN_ATTRIB;

        std::clog << "Calling inotify_add_watch( inotify_fd = " << inotify_fd
                << ", filename = " << std::quoted( filename ) << " )" << std::endl;

        auto const wd = inotify_add_watch( inotify_fd, filename.c_str(), flags );
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

        std::clog << "Calling inotify_rm_watch( inotify_fd = " << inotify_fd << ", wd = " << wd << " )";
        auto const result = inotify_rm_watch( inotify_fd, wd );
        checkForPosixError( result, "inotify_rm_watch()" );

        file_watchers.erase( filename );
    }
}

void FileWatcher::executeActionForWD( int const event_wd )
{
    std::lock_guard const locker( file_watchers_mutex );

    auto const p = std::ranges::find_if( file_watchers, [ event_wd ]( auto const& map_entry )
    {
        auto const& [ filename, wdaction ] = map_entry;
        auto const& [ wd, action ] = wdaction;

        return wd == event_wd;
    } );

    if ( p != file_watchers.end() )
    {
        auto const& [ filename, wdaction ] = *p;
        auto const& [ wd, action ] = wdaction;

        std::invoke( action );
    }
}

void FileWatcher::handleINotification()
{
    std::array< char, sizeof ( inotify_event ) + NAME_MAX + 1 > buffer;
    auto const num_bytes = read( inotify_fd, buffer.data(), buffer.size() );
    checkForPosixError( num_bytes, "read()" );

    auto const& event = *reinterpret_cast< inotify_event const* >( buffer.data() );
    std::clog << "event.wd = " << event.wd << std::endl;
    std::clog << "event.mask = " << event_mask_to_string( event.mask ) << std::endl;
    std::clog << "event.cookie = " << event.cookie << std::endl;
    std::clog << "event.len = " << event.len << std::endl;
    std::clog << "event.name = " << std::quoted( std::string_view( event.name, event.len ) ) << std::endl;

    // lookup which action to execute for this wd and execute it

    executeActionForWD( event.wd );
}

std::string FileWatcher::event_mask_to_string( int const event_mask )
{
    static Mappings const mappings
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

    return bitmaskToString( event_mask, mappings );
}
