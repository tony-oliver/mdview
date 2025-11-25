#ifndef INCLUDED_SEARCH_DIALOG_HPP
#define INCLUDED_SEARCH_DIALOG_HPP

#include <gtkmm.h>          // Gtk::*

#include <webkit/webkit.h>  // WebKitFindOptions

#include <memory>           // std::shared_ptr<>}{
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
    Gtk::CheckButton forward_button;
    Gtk::CheckButton backward_button;

    Gtk::Frame options_frame;
    Gtk::Box options_box;
    Gtk::CheckButton case_insensitive;
    Gtk::CheckButton match_at_word_starts;
    Gtk::CheckButton capitals_in_words;
    Gtk::CheckButton search_wraps_around;

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
    bool on_key_event( unsigned const keyval, unsigned keycode, Gdk::ModifierType state );
};

#endif // INCLUDED_SEARCH_DIALOG_HPP
