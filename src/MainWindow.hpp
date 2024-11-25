#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"

#include <gtkmm/window.h>

#include "WebView.hpp"
#include "FileWatcher.hpp"
#include "SignalHandler.hpp"
#include "ThreadSafeOStream.hpp"

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
