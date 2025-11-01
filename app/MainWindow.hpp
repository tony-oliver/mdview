#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"              // for Options{}
#include "MarkdownView.hpp"         // for MarkdownView{}
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

    std::ostream&       logger;
    ThreadSafeOStream   makeThreadSafe;
    MarkdownView        markdownView;

    void postProcess( std::string& html );

    void displayMarkdownFile();
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
