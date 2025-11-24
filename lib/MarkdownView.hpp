#ifndef INCLUDED_MARKDOWN_VIEW_HPP
#define INCLUDED_MARKDOWN_VIEW_HPP

#include "WebView.hpp"
#include "FileWatcher.hpp"
#include "SearchDialog.hpp"
#include "FindController.hpp"

#include <string>       // std::string{}
#include <ostream>      // std::ostream{}

class MarkdownView: public WebKit::WebView
{
    bool const dump_html;
    bool const show_diagnostics;
    std::string const& filename;
    FileWatcher file_watcher;
    std::shared_ptr< Gtk::EventControllerKey > keypress_tracker;
    WebKit::FindController find_controller;
    SearchDialog search_dialog;


public:

    MarkdownView(   std::ostream& logger,
                    std::string const& filename,
                    bool dump_html = false,
                    bool show_diagnostics = false );

    void render();

private:

    void post_process( std::string& html );

    std::string determine_window_title() const;

    bool on_key_pressed( unsigned keyval, unsigned keycode, Gdk::ModifierType state );

    void launch_search_dialog();
    void on_search();
};

#endif // INCLUDED_MARKDOWN_VIEW_HPP
