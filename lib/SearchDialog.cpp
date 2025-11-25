#include "SearchDialog.hpp"

#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

#include <cstdint>      // std::uint32_t
#include <utility>      // std::to_underlying<>()

// DEBUG!!!
#include <cassert>
#include <iomanip>
#include <ostream>
#include <iostream>

//============================================================================

SearchDialog::SearchDialog( Gtk::Window& parent )
: event_tracker(        Gtk::EventControllerLegacy::create() )
, find_box(             Gtk::Orientation::HORIZONTAL, 5 )
, find_label(           "Find:" )
, option_box(           Gtk::Orientation::VERTICAL )
, case_insensitive(     "Case-insensitive" )
, match_at_word_starts( "Match at word-starts" )
, capitals_in_words(    "Words start at capitals within words" )
, search_backwards(     "Search backwards" )
, search_wraps_around(  "Wrap search" )
, button_box(           Gtk::Orientation::HORIZONTAL, 5 )
, find_button(          "Find" )
, close_button(         "Close" )
, layout_box(           Gtk::Orientation::VERTICAL, 5 )
{
    add_controller( event_tracker );
    event_tracker ->signal_event().connect( sigc::mem_fun( *this, &SearchDialog::on_raw_event ), false );

    find_entry.set_expand();
    find_entry.signal_changed().connect( sigc::mem_fun( *this, &SearchDialog::on_search_text_changed ) );

    match_at_word_starts.signal_toggled().connect( sigc::mem_fun( *this, &SearchDialog::on_word_starts_toggled ) );
    capitals_in_words.set_sensitive( false );

    find_button.set_sensitive( false );
    find_button.signal_clicked().connect( sigc::mem_fun( *this, &SearchDialog::on_find_button_pressed ) );

    close_button.set_hexpand( true );
    close_button.set_halign( Gtk::Align::END );
    close_button.signal_clicked().connect( sigc::mem_fun( *this, &SearchDialog::on_close_button_pressed ) );

    find_box.set_margin( 5 );
    find_box.append( find_label );
    find_box.append( find_entry );

    option_frame.set_child( option_box );
    option_box.set_margin( 5 );
    option_box.append( case_insensitive );
    option_box.append( match_at_word_starts );
    option_box.append( capitals_in_words );
    option_box.append( search_backwards );
    option_box.append( search_wraps_around );

    button_box.set_margin( 5 );
    button_box.append( find_button );
    button_box.append( close_button );

    layout_box.set_margin( 5 );
    layout_box.append( find_box );
    layout_box.append( option_frame ); //option_box );
    layout_box.append( button_box );

    set_child( layout_box );
    set_title( "Search" );

    set_transient_for( parent );
    set_hide_on_close();
//    set_modal();
}

//----------------------------------------------------------------------------

void SearchDialog::set_search_action( SearchAction const action )
{
    search_action = action;
}

//----------------------------------------------------------------------------

std::string SearchDialog::get_search_text() const
{
    return find_entry.get_text();
}

//----------------------------------------------------------------------------

WebKitFindOptions SearchDialog::get_find_options() const
{
    std::uint32_t find_options = WEBKIT_FIND_OPTIONS_NONE;

    if ( case_insensitive.get_active() )
    {
        find_options |= WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE;
    }

    if ( match_at_word_starts.get_active() )
    {
        find_options |= WEBKIT_FIND_OPTIONS_AT_WORD_STARTS;

        if ( capitals_in_words.get_active() )
        {
            find_options |= WEBKIT_FIND_OPTIONS_TREAT_MEDIAL_CAPITAL_AS_WORD_START;
        }
    }

    if ( search_backwards.get_active() )
    {
        find_options |= WEBKIT_FIND_OPTIONS_BACKWARDS;
    }

    if ( search_wraps_around.get_active() )
    {
        find_options |= WEBKIT_FIND_OPTIONS_WRAP_AROUND;
    }

    return static_cast< WebKitFindOptions >( find_options );
}

//----------------------------------------------------------------------------

void SearchDialog::on_search_text_changed()
{
    auto const search_text = find_entry.get_text();

    find_button.set_sensitive( !search_text.empty() );
}

//----------------------------------------------------------------------------

void SearchDialog::on_word_starts_toggled()
{
    auto const word_starts_active = match_at_word_starts.get_active();

    if ( word_starts_active )
    {
        capitals_in_words.set_active( cached_capitals );
    }
    else
    {
        cached_capitals = capitals_in_words.get_active();
        capitals_in_words.set_active( false );
    }

    capitals_in_words.set_sensitive( word_starts_active );
}

//----------------------------------------------------------------------------

void SearchDialog::on_find_button_pressed()
{
    if ( search_action )
    {
        std::invoke( search_action );
    }
}

//----------------------------------------------------------------------------

void SearchDialog::on_close_button_pressed()
{
    set_visible( false );
}

//----------------------------------------------------------------------------

bool SearchDialog::on_raw_event( std::shared_ptr< Gdk::Event const > const& event )
{
    bool event_handled = false;

    if ( event->get_event_type() == Gdk::Event::Type::KEY_RELEASE )
    {
        event_handled = on_key_event( event->get_keyval(),
                                      event->get_keycode(),
                                      event->get_consumed_modifiers() );
    }

    return event_handled;
}

//----------------------------------------------------------------------------

bool SearchDialog::on_key_event( unsigned const keyval,
                                 unsigned /* const keycode */,
                                 Gdk::ModifierType const state )
{
    bool event_handled = false;

    if ( state == Gdk::ModifierType::NO_MODIFIER_MASK )
    {
        switch ( keyval )
        {
        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:

            if ( find_button.is_sensitive() )
            {
                find_button.activate();
            }

            event_handled = true;
            break;

        case GDK_KEY_Escape:

            close_button.activate();
            event_handled = true;
            break;
        }
    }

    return event_handled;
}

//============================================================================
