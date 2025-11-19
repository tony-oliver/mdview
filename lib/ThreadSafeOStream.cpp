#include "ThreadSafeOStream.hpp"

#include <unistd.h>         // isatty()

#include <ncurses/term.h>   // setupterm(), tparm(), tigetstr()
#include <ncurses/curses.h> // COLOR_CYAN, etc.

#include <iomanip>
#include <iostream>
#include <iterator>

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

auto ThreadSafeOStream::overflow( int_type const ch ) -> int_type
{
    char const c = ch;
    xsputn( &c, 1 );
    return ch;
}

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

void ThreadSafeOStream::flushBuffer( std::thread::id const thread_id, std::string& buffer, std::streamsize const length )
{
    if ( using_colours )
    {
        auto const colour_no = get_colour_no_for( thread_id );

        std::string terminal_str;
        terminal_str += tparm( tigetstr( "setaf" ), colour_no );
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

std::size_t ThreadSafeOStream::get_colour_no_for( std::thread::id const thread_id )
{
    int colour_no;

    auto const p = colours_in_use.find( thread_id );

    if ( p != colours_in_use.end() )
    {
        colour_no = p->second;
    }
    else
    {
        auto const num_colours_used = colours_in_use.size();

        constexpr int colours[] =
        {
            COLOR_CYAN,
            COLOR_MAGENTA,
            COLOR_GREEN,
            COLOR_BLUE,
            COLOR_YELLOW,
            COLOR_RED,
            COLOR_WHITE,
        };

        constexpr auto max_num_colours = std::size( colours );

        if ( num_colours_used < max_num_colours )
        {
            colour_no = colours[ num_colours_used ];
        }
        else
        {
            auto const hash = std::hash< std::thread::id >{}( thread_id );
            auto const colour_index = hash % max_num_colours;
            colour_no = colours[ colour_index ];
        }

        colours_in_use[ thread_id ] = colour_no;
    }

    return colour_no;
}
