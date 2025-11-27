#ifndef INCLUDED_SEARCH_DIALOG_HPP
#define INCLUDED_SEARCH_DIALOG_HPP

#include "SuperCheckButton.hpp"

#include <gtkmm.h>          // Gtk::*

#include <webkit/webkit.h>  // WebKitFindOptions

#include <string>           // std::string{}
#include <functional>       // std::function<>{}

class SearchDialog: public Gtk::Dialog // Gtk::Window
{
    std::shared_ptr< Gtk::EventControllerLegacy > event_tracker;

    Gtk::Box find_box;
    Gtk::Label find_label;
    Gtk::Entry find_entry;

    Gtk::Frame direction_frame;
    Gtk::Box direction_box;
    SuperCheckButton forward_button;
    SuperCheckButton backward_button;

    Gtk::Frame options_frame;
    Gtk::Box options_box;
    SuperCheckButton case_insensitive;
    SuperCheckButton match_at_word_starts;
    SuperCheckButton capitals_in_words;
    SuperCheckButton search_wraps_around;

    Gtk::Box button_box;
    Gtk::Button find_button;
    Gtk::Button close_button;

    Gtk::Box layout_box;

    using SearchAction = std::function< void() >;
    SearchAction search_action;

    bool cached_capitals = false;

public:

    explicit SearchDialog( Gtk::Window& parent );

    void set_search_action( SearchAction action );

    std::string get_search_text() const;
    WebKitFindOptions get_find_options() const;

private:

    void on_search_text_changed();
    void on_word_starts_toggled();
    void on_find_button_pressed();
    void on_close_button_pressed();

    bool on_raw_event( std::shared_ptr< Gdk::Event const > const& event );
    bool on_key_released( unsigned const keyval, unsigned keycode, Gdk::ModifierType state );

    void push_find_button();
    void push_close_button();
};

#endif // INCLUDED_SEARCH_DIALOG_HPP
