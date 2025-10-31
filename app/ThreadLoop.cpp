#include "ThreadLoop.hpp"
#include "StringUtils.hpp"

#include <unistd.h>         // pipe(), close()
#include <poll.h>           // poll_fd{}, poll()

#include <cerrno>           // errno
#include <string>           // std::string{}
#include <vector>           // std::vector<>{}
#include <iomanip>          // std::setfill()
#include <ostream>          // std::endl, operator<<()
#include <sstream>          // std::ostringstream{}
#include <iostream>         // std::cerr{}
#include <iterator>         // std::data<>(), std::size<>()
#include <algorithm>        // std::find_if<>()
#include <string_view>      // std::sstring_view{}
#include <system_error>     // std::system_error{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

std::string pollEventsToString( int const events )
{
    static awo::StringMappings const mappings
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

    return awo::bitmaskToString( events, mappings );
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================
namespace awo {
//----------------------------------------------------------------------------

ThreadLoop::ThreadLoop( std::ostream& logger )
: logger{ logger }
{
    logger << "ThreadLoop ctor entered..." << std::endl;

    auto const pipe_result = pipe( stop_pipe.data() );
    checkForPosixError( pipe_result, "pipe()" );

    logger << "Stop-pipe: writing-fd = " << stop_pipe[ WriteEnd ]
           << "; reading-fd = " << stop_pipe[ ReadEnd ] << std::endl;

    logger << "ThreadLoop ctor done." << std::endl;
}

//----------------------------------------------------------------------------

ThreadLoop::~ThreadLoop()
{
    logger << "ThreadLoop dtor entered..." << std::endl;

    stop();

    closeStopPipe( ReadEnd, "ReadEnd" );

    logger << "ThreadLoop dtor done." << std::endl;
}

//----------------------------------------------------------------------------

bool ThreadLoop::running() const
{
    return polling_thread.joinable();
}

//----------------------------------------------------------------------------

void ThreadLoop::start()
{
    if ( running() )
    {
        logger << "ThreadLoop already running - ignoring start() request" << std::endl;
    }
    else
    {
        logger << "Creating thread running the pollingLoop() function" << std::endl;
        polling_thread = std::thread( [ this ]{ pollingLoop(); } );
    }
}

//----------------------------------------------------------------------------

void ThreadLoop::stop()
{
    if ( running() )
    {
        logger << "Setting stopping = true" << std::endl;
        stopping = true;

        closeStopPipe( WriteEnd, "WriteEnd" );

        logger << "Waiting for polling-thread to finish" << std::endl;
        polling_thread.join();
        logger << "Polling-thread has joined" << std::endl;
    }
    else
    {
        logger << "ThreadLoop is not running - ignoring stop() request" << std::endl;
    }
}

//----------------------------------------------------------------------------

void ThreadLoop::closeStopPipe( PipeEnd const pipeEnd, std::string const& endName )
{
    auto const fd = stop_pipe[ pipeEnd ];
    logger << "Closing stop_pipe[" << endName << "] = fd " << fd << " ... ";
    auto const close_result = close( fd );
    checkForPosixError( close_result, "close()" );
}

//----------------------------------------------------------------------------

void ThreadLoop::registerActionForFD( int const fd, Action const& action )
{
    std::lock_guard const locker( action_table_mutex );

    action_table[ fd ] = action;
}

//----------------------------------------------------------------------------

void ThreadLoop::deregisterFD( int const fd )
{
    std::lock_guard const locker( action_table_mutex );

    action_table.erase( fd );
}

//----------------------------------------------------------------------------

void ThreadLoop::executeActionForFD( int const fd )
{
    std::lock_guard const locker( action_table_mutex );

    if ( action_table.contains( fd ) )
    {
        // Lookup the registered action for the given fd and execute it
        std::invoke( action_table.at( fd ) );
    }
}

//----------------------------------------------------------------------------

void ThreadLoop::pollingLoop()
{
    logger << "Function " << __FUNCTION__ << "() now executing" << std::endl;

    constexpr int no_timeout = -1;

    try
    {
        do // Loop, waiting for events to occur and servicing them.
        {
            std::vector< pollfd > pollfds;

            logger << "Adding stop-fd " << stop_pipe[ ReadEnd ] << " to pollfds[]" << std::endl;
            pollfds.emplace_back( stop_pipe[ ReadEnd ], POLLIN, 0 );

            for ( auto const& [ fd, action ]: action_table )
            {
                logger << "Adding registered fd " << fd << " to pollfds[]" << std::endl;
                pollfds.emplace_back( fd, POLLIN, 0 );
            }

            logger << "Calling poll()" << std::endl;
            //-------------------------------------------------------------------------
            auto const poll_result = poll( pollfds.data(), pollfds.size(), no_timeout );
            //-------------------------------------------------------------------------
            if ( stopping ) break;
            if ( checkForPosixError( poll_result, "poll()" ) ) continue; // loop again if we got an EINTR "error"

            // Now go through the pollfds to see which one(s) triggered the poll() to return

            for ( auto const& pollfd: pollfds )
            {
                if ( pollfd.revents != 0 )
                {
                    logger << "pollfds[ fd = " << pollfd.fd << " ].revents == " << pollEventsToString( pollfd.revents ) << std::endl;

                    if ( ( pollfd.revents & POLLIN ) != 0 )
                    {
                        logger << "POLLIN flag found in revents." << std::endl;

                        // This pollfd has read-events to be serviced.

                        if ( pollfd.fd == stop_pipe[ ReadEnd ] ) break; // Halt this loop so the 'stopping' flag can be re-checked

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

    logger << "Function " << __FUNCTION__ << "() now exiting" << std::endl;
}

//----------------------------------------------------------------------------

bool ThreadLoop::checkForPosixError( std::intmax_t const value, std::string const& what )
{
    logger << what << " returned " << value << std::endl;

    if ( value == -1 )
    {
        if ( errno == EINTR ) return true;

        throw std::system_error( errno, std::generic_category(), what );
    }

    return false;
}
//----------------------------------------------------------------------------
} // close namespace awo
//============================================================================
