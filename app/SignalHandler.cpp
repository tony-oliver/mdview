#include "SignalHandler.hpp"

#include <csignal>      // SIG*, std::signal()
#include <fstream>      // std::ofstream{}
#include <ostream>
#include <utility>      // std::exchange<>()

//============================================================================
namespace awo {
//----------------------------------------------------------------------------

SignalHandler::Signals const SignalHandler::default_signos
{
    SIGINT,     // [ISO C99]    Interactive attention signal
    SIGTERM,    // [ISO C99]    Termination request
    SIGQUIT,    // [POSIX]      Quit
    SIGHUP,     // [POSIX]      Hangup
    SIGPWR,     // [Linux]      Power failure imminent
};

//----------------------------------------------------------------------------

SignalHandler::SignalHandler( Signals const& signos )
{
    // Point every sensitive signal to our signal-handler.

    for ( auto signo: signos )
    {
        std::signal( signo, handleSignal );
    }
}

//----------------------------------------------------------------------------

void SignalHandler::registerAction( Action const& newAction )
{
    // Record the action to be performed on receipt of any sensitive signal.

    action = newAction;
}

//----------------------------------------------------------------------------

void SignalHandler::handleSignal( int const signo )
{
    if ( signo == SIGINT )
    {
        // "^C" has probably been printed; throw a newline, too.

        std::ofstream( "/dev/tty" ) << std::endl;
    }

    // Get the action provided in registerAction() and clear it.

    auto const signal_handler{ std::exchange( action, {} ) };

    // If we found an action to perform, invoke it.

    if ( signal_handler != nullptr )
    {
        std::invoke( signal_handler );
    }
}

//----------------------------------------------------------------------------
} // close namespace awo
//============================================================================
