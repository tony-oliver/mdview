#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"              // for Options{}
#include "MarkdownView.hpp"         // for MarkdownView{}
#include "SignalHandler.hpp"        // for awo::SignalHandler{}
#include "ThreadSafeOStream.hpp"    // for ThreadSafeOStream{}

#include <gtkmm.h>                  // for Gtk::Window{}

#include <string>

class MainWindow: public Gtk::Window
{
public:

    explicit MainWindow( Options const& options );

private:

    awo::SignalHandler  signalHandler;

    std::ostream&       logger;
    std::string const&  filename;
    ThreadSafeOStream   makeThreadSafe;
    MarkdownView        markdownView;

    std::string determine_window_title() const;
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
