#include "MainWindow.hpp"

#include <gtkmm/application.h>

#include <string>
#include <vector>

int main( int const argc, char** const argv )
{
    MainWindow::Arguments const args( argv + 1, argv + argc );

    Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, args );
}
