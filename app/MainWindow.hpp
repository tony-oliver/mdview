#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"              // for Options{}
#include "WebView.hpp"              // for Webkit::WebView{}
#include "FileWatcher.hpp"          // for FileWatcher{}
#include "SignalHandler.hpp"        // for awo::SignalHandler{}
#include "ThreadSafeOStream.hpp"    // for ThreadSafeOStream{}

#include <gtkmm/window.h>           // for Gtk::Window{}

#include <string>

class MainWindow: public Gtk::Window
{
public:

    explicit MainWindow( Options const& options );

private:

    awo::SignalHandler  signalHandler;

    Options const&      options;
    std::string const&  filename;
    std::ostream&       logger;

    WebKit::WebView     webView;
    ThreadSafeOStream   makeThreadSafe;
    FileWatcher         watcher;

    void postProcess( std::string& html );

    void displayMarkdownFile();
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
