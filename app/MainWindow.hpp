#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "Options.hpp"              // Options{}
#include "MarkdownView.hpp"         // MarkdownView{}
#include "SignalHandler.hpp"        // awo::SignalHandler{}
#include "ThreadSafeOStream.hpp"    // ThreadSafeOStream{}

#include <gtkmm.h>                  // Gtk::Window{}

#include <string>                   // std::string{}
#include <ostream>                  // std::ostream{}

class MainWindow: public Gtk::Window
{
    awo::SignalHandler              signal_handler;
    std::ostream&                   logger;
    std::string const&              filename;
    ThreadSafeOStream               make_thread_safe;
    MarkdownView                    markdown_view;

public:

    explicit MainWindow( Options const& options );
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
