#include "Options.hpp"          // for Options{}
#include "MainWindow.hpp"       // for MainWindow{}

#include <gtkmm/application.h>  // for Gtk::Application{}

int main( int const argc, char** const argv )
{
    // Collect options/arguments specified on the command-line
    Options const options( argc, argv );

    // NOTE: exceptions thrown in the following call will *not* be propagated out to here
    Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
}
