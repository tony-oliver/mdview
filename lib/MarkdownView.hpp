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
    std::shared_ptr< Gtk::GestureClick > click_tracker;
    WebKit::FindController find_controller;
    SearchDialog search_dialog;
    Gtk::Window* parent = nullptr;

public:

    MarkdownView(   Gtk::Window& parent,
                    std::ostream& logger,
                    std::string const& filename,
                    bool dump_html = false,
                    bool show_diagnostics = false );

    void set_parent( Gtk::Window* window );
    void render();

private:

    std::string determine_window_title() const;
    void post_process_html( std::string& html );

    void launch_search_dialog();
    void on_search();

    bool on_key_pressed( unsigned keyval, unsigned keycode, Gdk::ModifierType state );
    void on_unpaired_button_release( double x, double y, unsigned button, Gdk::EventSequence* sequence );
};

#endif // INCLUDED_MARKDOWN_VIEW_HPP
