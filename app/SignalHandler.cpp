#include "SignalHandler.hpp"

#include <csignal>
#include <fstream>
#include <ostream>
#include <utility>

//============================================================================
namespace awo {
//----------------------------------------------------------------------------

SignalHandler::SignalHandler( std::set< int > const& signos )
{
    for ( auto signo: signos )
    {
        std::signal( signo, handleSignal );
    }
}

//----------------------------------------------------------------------------

void SignalHandler::registerAction( Action const& newAction )
{
    action = newAction;
}

//----------------------------------------------------------------------------

void SignalHandler::handleSignal( int const signo )
{
    if ( signo == SIGINT )
    {
        std::ofstream( "/dev/tty" ) << std::endl;
    }

    if ( auto const callable{ std::exchange( action, {} ) } )
    {
        std::invoke( callable );
    }
}

//----------------------------------------------------------------------------
} // close namespace awo
//============================================================================
