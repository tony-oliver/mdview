#include "SuperCheckButton.hpp"
#include "KeyMatch.hpp"

// DEBUG!!!
#include <ostream>
#include <iostream>

//============================================================================

SuperCheckButton::SuperCheckButton( std::string const& caption,
                                    EnterAction const enter_action )
: Gtk::CheckButton( caption )
, enter_action{ enter_action }
, keypress_tracker{ Gtk::EventControllerKey::create() }
{
    add_controller( keypress_tracker );
    keypress_tracker->signal_key_pressed().connect( sigc::mem_fun( *this, &SuperCheckButton::on_key_pressed ), false );
}

//----------------------------------------------------------------------------

bool SuperCheckButton::on_key_pressed( unsigned const keyval,
                                       unsigned /* const keycode */,
                                       Gdk::ModifierType const state )
{
    // NOTE: if this table is static, 'this' is captured just once (on the first
    // widget to get here) - this causes trouble if there are multiple SuperCheckButtons,
    // so we must build this KeyMatch table every time we are called.
    KeyMatch const key_matches[]
    {
        { GDK_KEY_Return,   Gdk::ModifierType::NO_MODIFIER_MASK, [ & ]{ do_enter_action(); } },
        { GDK_KEY_KP_Enter, Gdk::ModifierType::NO_MODIFIER_MASK, [ & ]{ do_enter_action(); } },
        {}
    };

    return match_key( "SuperCheckButton::on_key_pressed()", keyval, state, key_matches );
}

//----------------------------------------------------------------------------

void SuperCheckButton::do_enter_action()
{
    if ( enter_action )
    {
        enter_action();
    }
}

//============================================================================
