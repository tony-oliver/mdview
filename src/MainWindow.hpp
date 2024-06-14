#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "WebView.hpp"
#include "FileWatcher.hpp"
#include "SignalHandler.hpp"

#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>

#include <string>
#include <vector>
#include <cstddef>
#include <string_view>

class MainWindow: public Gtk::Window
{
public:

    using Arguments = std::vector< std::string >;

    explicit MainWindow( Arguments const& args );

private:

    Gtk::ScrolledWindow scroller;
    WebView             webView;
    std::string         filename;
    FileWatcher         watcher;
    SignalHandler       signalHandler;

    void display( Arguments const& args = {} );
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
