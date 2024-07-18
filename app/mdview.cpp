#include "MainWindow.hpp"
#include "Options.hpp"

#include <gtkmm/application.h>

namespace { // unnamed

class App: public Gtk::Application
{
public:

    explicit App( Gio::Application::Flags const flags = Gio::Application::Flags::DEFAULT_FLAGS )
    : Gtk::Application( {}, flags )
    {
    }
};

} // close unnamed namespace

int main( int const argc, char** const argv )
{
    Options const options( argc, argv );

//#define USE_OWN_APP
#ifdef USE_OWN_APP
    App app( Gio::Application::Flags::HANDLES_COMMAND_LINE );

    return app.make_window_and_run< MainWindow >( 0, nullptr, options );
#else
    Gtk::Application::create()->make_window_and_run< MainWindow >( 1, argv, options );
#endif
}
