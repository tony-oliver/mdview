#include "SignalHandler.hpp"

#include <csignal>      // SIG*, std::signal()
#include <fstream>      // std::ofstream{}
#include <ostream>      // std::ostream{}, std::endl()
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
    // Attach every signal-of-interest to our signal-handler.

    for ( auto signo: signos )
    {
        std::signal( signo, handleSignal );
    }
}

//----------------------------------------------------------------------------

void SignalHandler::registerAction( Action const& newAction )
{
    // Record the action to be performed on receipt of any signal of interest.

    action = newAction;
}

//----------------------------------------------------------------------------

void SignalHandler::handleSignal( int const signo )
{
    if ( signo == SIGINT )
    {
        // "^C" has probably been printed.  Throw a newline, too.

        std::ofstream( "/dev/tty" ) << std::endl;
    }

    // Get the action previously provided in registerAction() and clear it.

    auto const signal_handler{ std::exchange( action, {} ) };

    // If we found an action to perform, invoke it.

    if ( signal_handler != nullptr )
    {
        signal_handler();
    }
}

//----------------------------------------------------------------------------
} // close namespace awo
//============================================================================
