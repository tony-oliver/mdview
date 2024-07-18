#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"
#include "FileWatcher.hpp"
#include "SignalHandler.hpp"
#include "WebViewWidget.hpp"
#include "ThreadSafeOStream.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/scrolledwindow.h>

#include <string>
#include <vector>
#include <cstddef>
#include <string_view>

class MainWindow: public Gtk::ApplicationWindow
{
public:

    explicit MainWindow( Options const& options );

private:

    Options const&      options;
    Gtk::ScrolledWindow scroller;
    WebViewWidget       webViewWidget;
    ThreadSafeOStream   threadsafe_logger;
    FileWatcher         watcher;
    awo::SignalHandler  signalHandler;

    void display();
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
