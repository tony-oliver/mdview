#include "Options.hpp"          // Options{}
#include "MainWindow.hpp"       // MainWindow{}

#include <gtkmm.h>              // Gtk::Application{}

//============================================================================

int main( int const argc, char** const argv )
{
    Options const options( argc, argv );

    auto const application = Gtk::Application::create();

    return application->make_window_and_run< MainWindow >( 1, argv, options );
}

//============================================================================
