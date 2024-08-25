#include "Options.hpp"
#include "MainWindow.hpp"

#include <gtkmm/application.h>

int main( int const argc, char** const argv )
{
    Options const options( argc, argv );

    Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
}
