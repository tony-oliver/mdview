#ifndef INCLUDED_SUPER_CHECK_BUTTON_HPP
#define INCLUDED_SUPER_CHECK_BUTTON_HPP

#include <gtkmm.h>

#include <string>
#include <functional>

/// This class acts like a Gtk::CheckButton, but prevents the
/// underlying widget from seeing any Return/Enter keystrokes.

class SuperCheckButton: public Gtk::CheckButton
{
    using EnterAction = std::function< void() >;
    EnterAction enter_action;

    std::shared_ptr< Gtk::EventControllerKey > keypress_tracker;

public:

    explicit SuperCheckButton( std::string const& caption,
                               EnterAction enter_action = {} );

private:

    bool on_key_pressed( unsigned keyval,
                         unsigned keycode,
                         Gdk::ModifierType state );

    void do_enter_action();
};

#endif // INCLUDED_SUPER_CHECK_BUTTON_HPP
