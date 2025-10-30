#include "Options.hpp"          // Options{}
#include "MainWindow.hpp"       // MainWindow{}

#include <gtkmm/application.h>  // Gtk::Application{}

#include <unistd.h>             // daemon()

#include <cerrno>               // errno
#include <cstdlib>              // EXIT_SUCCESS, EXIT_FAILURE
#include <ostream>              // std::endl()
#include <iostream>             // std::cerr
#include <exception>            // std::exception{}
#include <system_error>         // std::system_error{}, std::generic_category()

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

void detach_unless_declined( Options const& options )
{
    if ( !options.get_foreground() )
    {
        auto& logger = options.get_logger();

        logger << "This is the original process (PID " << getpid() << ")" << std::endl;

        if ( daemon( true, true ) == -1 )
        {
            throw std::system_error( errno, std::generic_category(), "daemon()" );
        }

        logger << "This is the child (daemon) process (PID " << getpid() << ")" << std::endl;
    }
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

int main( int const argc, char** const argv )
{
    int exit_code = EXIT_FAILURE; // presumed guilty until proven innocent

    try
    {
        // Collect options/arguments specified on the command-line.
        Options const options( argc, argv );

        // Go into the background unless directed not to (by option --foreground).
        detach_unless_declined( options );

        // NOTE: exceptions thrown in the following call will *not* be propagated out to here.
        exit_code = Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
    }
    catch ( std::exception const& e )
    {
        // Report exceptions thrown in options-gathering or daemoization.
        std::cerr << "* " << e.what() << std::endl;
    }

    return exit_code;
}
