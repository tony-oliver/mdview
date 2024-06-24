#include "MainWindow.hpp"
#include "Options.hpp"

#include <gtkmm/application.h>

int main( int const argc, char** const argv )
{
    Options const options( argc, argv );

    return Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
}
