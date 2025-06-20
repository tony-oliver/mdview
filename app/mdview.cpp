#include "Options.hpp"          // for Options{}
#include "MainWindow.hpp"       // for MainWindow{}

#include <gtkmm/application.h>  // for Gtk::Application{}

#include <unistd.h>             // for daemon()

#include <cerrno>               // for errno
#include <cstdlib>              // for EXIT_SUCCESS, EXIT_FAILURE
#include <ostream>              // for std::endl()
#include <iostream>             // for std::cerr
#include <exception>            // for std::exception{}
#include <system_error>         // for std::system_error{}, std::generic_category()

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
    int exit_code = EXIT_FAILURE; // presumed guilty until proved innocent

    try
    {
        // Collect options/arguments specified on the command-line
        Options const options( argc, argv );

        // Go into the background unless directed not to (by option --foreground)
        detach_unless_declined( options );

        // NOTE: exceptions thrown in the following call will *not* be propagated out to here
        exit_code = Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
    }
    catch ( std::exception const& e )
    {
        std::cerr << "* " << e.what() << std::endl;
    }

    return exit_code;
}
