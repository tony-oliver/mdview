#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"
#include "FileWatcher.hpp"
#include "SignalHandler.hpp"
#include "WebViewWidget.hpp"
#include "ThreadSafeOStream.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/scrolledwindow.h>

#include <memory>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <string_view>

class MainWindow: public Gtk::ApplicationWindow
{
public:

    explicit MainWindow( Options const& options );

private:

    awo::SignalHandler  signalHandler;

    Options const&      options;
    std::string const&  filename;
    std::ostream&       logger;

    Gtk::ScrolledWindow scroller;
    WebViewWidget       webViewWidget;
    ThreadSafeOStream   makeThreadSafe;
    FileWatcher         watcher;

    void postProcess( std::string& html );
    void displayMarkdownFile();
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
