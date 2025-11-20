#include "ThreadSafeOStream.hpp"

#include <term.h>           // setupterm(), tparm(), tigetstr()
#include <curses.h>         // COLOR_CYAN, etc.

#include <unistd.h>         // isatty(), STDOUT_FILENO, STDERR_FILENO, gettid()

#include <iostream>         // std::cout, std::cerr, std::clog

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

bool is_attached_to_terminal( std::streambuf const* const stream_buffer )
{
    std::map< std::streambuf const*, int const > const streambuf_map
    {
        { std::cout.rdbuf(), STDOUT_FILENO },
        { std::cerr.rdbuf(), STDERR_FILENO },
        { std::clog.rdbuf(), STDERR_FILENO },
    };

    auto const p = streambuf_map.find( stream_buffer );
    auto const fd = ( p == streambuf_map.end() )? -1: p->second;
    return isatty( fd );
}

//----------------------------------------------------------------------------

constexpr int colours[]
{
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_RED,
    COLOR_WHITE,
};

constexpr auto num_actual_colours = std::size( colours );

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

ThreadSafeOStream::ThreadSafeOStream( std::ostream& stream, bool const use_colours )
: host_ostream{ stream }
, underlying_streambuf{ stream.rdbuf() }
, using_colours{ use_colours && is_attached_to_terminal( underlying_streambuf ) }
{
    if ( using_colours )
    {
        setupterm( nullptr, -1, nullptr );
    }

    // Set this instance of our streambuf variant into the host ostream.
    host_ostream.rdbuf( this );

    // Force the underlying stream to write everything straight through,
    // thereby obviating the need to call pubsync() on it.
    underlying_streambuf->pubsetbuf( nullptr, 0 );

    // Disable traditional buffering in this streambuf instance;
    // this forces sputc() to always call overflow(), which is overridden.
    pubsetbuf( nullptr, 0 );
}

//----------------------------------------------------------------------------

ThreadSafeOStream::~ThreadSafeOStream()
{
    std::unique_lock const locker( buffers_mutex );

    for ( auto& [ thread_id, buffer ]: buffers )
    {
        if ( !buffer.empty() )
        {
            flushBuffer( thread_id, buffer, buffer.length() );
        }
    }

    host_ostream.rdbuf( underlying_streambuf );
}

//----------------------------------------------------------------------------

auto ThreadSafeOStream::overflow( int_type const ch ) -> int_type
{
    char const c = ch;
    xsputn( &c, 1 );
    return ch;
}

//----------------------------------------------------------------------------

std::streamsize ThreadSafeOStream::xsputn( char const* const s, std::streamsize const count )
{
    std::lock_guard const locker( buffers_mutex );

    auto const thread_id = std::this_thread::get_id();
    auto& buffer = buffers[ thread_id ];
    buffer.append( s, count );

    for ( ;; )
    {
        auto const length = buffer.find( '\n' ) + 1;

        if ( length == 0 ) break;

        flushBuffer( thread_id, buffer, length );
    }

    return count;
}

//----------------------------------------------------------------------------

void ThreadSafeOStream::flushBuffer( std::thread::id const thread_id, std::string& buffer, std::streamsize const length )
{
    if ( using_colours )
    {
        auto const colour = get_colour_for_thread( thread_id );

        std::string terminal_str;
        terminal_str += tparm( tigetstr( "setaf" ), colour );
        terminal_str += tparm( tigetstr( "bold" ) );
        underlying_streambuf->sputn( terminal_str.data(), terminal_str.size() );
    }

    // Output the relevant segment of the buffer
    underlying_streambuf->sputn( buffer.c_str(), length );

    if ( using_colours )
    {
        std::string terminal_str;
        terminal_str += tparm( tigetstr( "sgr0" ) );
        underlying_streambuf->sputn( terminal_str.data(), terminal_str.size() );
    }

    // Remove the data that was output
    buffer.erase( 0, length );
}

//----------------------------------------------------------------------------

std::size_t ThreadSafeOStream::get_colour_for_thread( std::thread::id const thread_id )
{
    // If we've seen this thread before, return the colour we allocated.
    auto const p = thread_colours.find( thread_id );

    if ( p != thread_colours.end() )
    {
        return p->second;
    }

    // If insufficient threads have been allocated colours to have
    // exhausted the colour-list, allocate the next one available.
    auto colour_index = thread_colours.size();

    if ( colour_index >= num_actual_colours )
    {
        // All the colours have been used at least once;
        // "randomly" select one to be re-used for this thread.

        auto const hash = std::hash< std::thread::id >{}( thread_id );
        colour_index = hash % num_actual_colours;
    }

    return thread_colours[ thread_id ] = colours[ colour_index ];
}

//============================================================================
