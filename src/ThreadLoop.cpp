#include "ThreadLoop.hpp"

#include <unistd.h>         // pipe(), close()
#include <poll.h>           // poll_fd{}, poll()

#include <array>            // std::array<>{}
#include <cerrno>           // errno
#include <vector>           // std::vector<>{}
#include <iomanip>          // std::setfill()
#include <ostream>          // std::endl, operator<<()
#include <sstream>          // std::ostringstream{}
#include <iostream>         // std::clog{}
#include <iterator>         // std::data<>(), std::size<>()
#include <algorithm>        // std::find_if<>()
#include <string_view>      // std::sstring_view{}
#include <system_error>     // std::system_error{}

ThreadLoop::ThreadLoop()
{
    std::clog << "Creating pipe." << std::endl;
    auto const pipe_result = pipe( stop_fds.data() );
    checkForPosixError( pipe_result, "pipe()" );

    std::clog << "Stop-pipe: read from fd " << stop_fds[ ReadEnd ]
              << ";  write to fd " << stop_fds[ WriteEnd ] << std::endl;
}

ThreadLoop::~ThreadLoop()
{
    stop();

    for ( auto const stop_fd: stop_fds )
    {
        std::clog << "Closing stop_fd " << stop_fd << std::endl;
        auto const close_result = close( stop_fd );
        checkForPosixError( close_result, "close()" );
    }
}

void ThreadLoop::start()
{
    std::clog << "Creating thread running the run() function" << std::endl;
    polling_thread = std::thread( [ this ]{ pollingLoop(); } );
}

void ThreadLoop::stop()
{
    std::clog << "Setting stopping = true" << std::endl;
    stopping = true;

    std::clog << "Writing 'x' to fd " << stop_fds[ WriteEnd ] << std::endl;
    std::array< char, 1 > const buffer{ 'x' };
    auto const write_result = write( stop_fds[ WriteEnd ], buffer.data(), buffer.size() );
    checkForPosixError( write_result, "write()" );

    if ( polling_thread.joinable() )
    {
        std::clog << "Waiting for polling-thread to finish" << std::endl;
        polling_thread.join();
    }
}

void ThreadLoop::registerActionForFD( int const fd, Action const& action )
{
    std::lock_guard const locker( action_table_mutex );

    if ( action_table.contains( fd ) )
    {
        throw std::logic_error( "ThreadLoop::registerFD( fd = " + std::to_string( fd ) + ", action ) - fd already registered." );
    }

    action_table[ fd ] = action;
}

void ThreadLoop::deregisterFD( int const fd )
{
    std::lock_guard const locker( action_table_mutex );

    if ( !action_table.contains( fd ) )
    {
        throw std::logic_error( "ThreadLoop::deregisterFD( fd = " + std::to_string( fd ) + " ) - fd wasn't registered." );
    }

    action_table.erase( fd );
}

void ThreadLoop::executeActionForFD( int const fd )
{
    std::lock_guard const locker( action_table_mutex );

    if ( action_table.contains( fd ) )
    {
        // Lookup the registered action for the given fd and execute it
        std::invoke( action_table.at( fd ) );
    }
}

void ThreadLoop::pollingLoop()
{
    std::clog << __FUNCTION__ << "() function now executing" << std::endl;

    constexpr int no_timeout = -1;

    try
    {
        // Loop, waiting for events to occur and servicing them.

        do
        {
            std::clog << std::endl;

            std::vector< pollfd > pollfds;

            std::clog << "Adding stop-fd " << stop_fds[ ReadEnd ] << " to pollfds[]" << std::endl;
            pollfds.emplace_back( stop_fds[ ReadEnd ], POLLIN, 0 );

            for ( auto const& [ fd, action ]: action_table )
            {
                std::clog << "Adding registered fd " << fd << " to pollfds[]" << std::endl;
                pollfds.emplace_back( fd, POLLIN, 0 );
            }

            std::clog << "Calling poll()" << std::endl;
            auto const poll_result = poll( pollfds.data(), pollfds.size(), no_timeout );

            if ( checkForPosixError( poll_result, "poll()" ) )
            {
                continue; // loop again if we got an EINTR "error"
            }

            // Now go through the pollfds to see which one(s) triggered the poll() to return

            for ( auto const& pollfd: pollfds )
            {
                if ( pollfd.revents != 0 )
                {
                    std::clog << "pollfds[ fd = " << pollfd.fd << " ].revents == " << eventsToString( pollfd.revents ) << std::endl;

                    if ( ( pollfd.revents & POLLIN ) != 0 )
                    {
                        std::clog << "POLLIN flag found in revents." << std::endl;

                        // This pollfd has read-events to be serviced.

                        if ( pollfd.fd == stop_fds[ ReadEnd ] )
                        {
                            break; // Halt this loop so the 'stopping' flag can be re-checked
                        }

                        executeActionForFD( pollfd.fd );
                    }
                }
            }
        }
        while ( !stopping );
    }
    catch ( std::exception const& e )
    {
        std::cerr << "* " << e.what() << std::endl;
    }

    std::clog << "Function " << __FUNCTION__ << "() now exiting" << std::endl;
}

bool ThreadLoop::checkForPosixError( std::intmax_t value, std::string const& what )
{
    std::clog << what << " returned " << value << std::endl;

    if ( value == -1 )
    {
        if ( errno == EINTR )
        {
            return true;
        }

        throw std::system_error( errno, std::generic_category(), what );
    }

    return false;
}

std::string ThreadLoop::eventsToString( int const events )
{
    static ThreadLoop::Mappings const mappings
    {
#define MAPPING( x ) { x, #x },
        MAPPING( POLLIN     )
        MAPPING( POLLPRI    )
        MAPPING( POLLOUT    )
        MAPPING( POLLERR    )
        MAPPING( POLLHUP    )
        MAPPING( POLLNVAL   )
        MAPPING( POLLRDNORM )
        MAPPING( POLLRDBAND )
        MAPPING( POLLWRNORM )
        MAPPING( POLLWRBAND )
        MAPPING( POLLMSG    )
        MAPPING( POLLREMOVE )
        MAPPING( POLLRDHUP  )
#undef  MAPPING
    };

    return bitmaskToString( events, mappings );
}

std::string ThreadLoop::bitmaskToString( int value, Mappings const& mappings )
{
    std::ostringstream oss;
    std::string delimiter;

    for ( auto const& [ mappedBitmask, name ]: mappings )
    {
        if ( ( value & mappedBitmask ) == mappedBitmask )
        {
            value ^= mappedBitmask;
            oss << delimiter << name;
            if ( delimiter.empty() ) delimiter = " | ";
        }
    }

    if ( value != 0 )
    {
        oss << delimiter << "0x" << std::hex << std::uppercase << std::setfill( '0' ) << std::setw( 8 ) << value;
    }

    return oss.str();
}
