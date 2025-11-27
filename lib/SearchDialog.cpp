#include "SearchDialog.hpp"
#include "KeyMatch.hpp"

#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

#include <cstdint>      // std::uint32_t
#include <utility>      // std::to_underlying<>()

//============================================================================

SearchDialog::SearchDialog( Gtk::Window& parent )
: event_tracker(        Gtk::EventControllerLegacy::create() )
, find_box(             Gtk::Orientation::HORIZONTAL, 5 )
, find_label(           "Find:" )
, direction_frame(      "Direction:" )
, direction_box(        Gtk::Orientation::HORIZONTAL, 5 )
, forward_button(       "Forward",                              [ & ]{ on_find_button_pressed(); } )
, backward_button(      "Backward",                             [ & ]{ on_find_button_pressed(); } )
, options_frame(        "Options:" )
, options_box(          Gtk::Orientation::VERTICAL )
, case_insensitive(     "Case-insensitive",                     [ & ]{ on_find_button_pressed(); } )
, match_at_word_starts( "Match at word-starts",                 [ & ]{ on_find_button_pressed(); } )
, capitals_in_words(    "Words start at capitals within words", [ & ]{ on_find_button_pressed(); } )
, search_wraps_around(  "Wrap search",                          [ & ]{ on_find_button_pressed(); } )
, button_box(           Gtk::Orientation::HORIZONTAL, 5 )
, find_button(          "Find" )
, close_button(         "Close" )
, layout_box(           Gtk::Orientation::VERTICAL, 5 )
{
    // We have to use the raw event handler (rather than a key-tracker handler) in order that
    // the search-text entry-box will still be escapable on detection of the Return key.
    add_controller( event_tracker );
    event_tracker ->signal_event().connect( sigc::mem_fun( *this, &SearchDialog::on_raw_event ), false );

    find_entry.set_expand();
    find_entry.signal_changed().connect( sigc::mem_fun( *this, &SearchDialog::on_search_text_changed ) );

    forward_button.set_active();
    backward_button.set_group( forward_button );

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

    direction_box.set_margin( 5 );
    direction_box.append( forward_button );
    direction_box.append( backward_button );
    direction_frame.set_margin( 5 );
    direction_frame.set_child( direction_box );

    options_box.set_margin( 5 );
    options_box.append( case_insensitive );
    options_box.append( match_at_word_starts );
    options_box.append( capitals_in_words );
    options_box.append( search_wraps_around );
    options_frame.set_margin( 5 );
    options_frame.set_child( options_box );

    button_box.set_margin( 5 );
    button_box.append( find_button );
    button_box.append( close_button );

    layout_box.set_margin( 5 );
    layout_box.append( find_box );
    layout_box.append( direction_frame );
    layout_box.append( options_frame );
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

    if ( backward_button.get_active() )
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
    if ( find_button.is_sensitive() )
    {
        if ( search_action )
        {
            std::invoke( search_action );
        }
    }
}

//----------------------------------------------------------------------------

void SearchDialog::on_close_button_pressed()
{
    if ( close_button.is_sensitive() )
    {
        set_visible( false );
    }
}

//----------------------------------------------------------------------------

bool SearchDialog::on_raw_event( std::shared_ptr< Gdk::Event const > const& event )
{
    bool event_handled = false;

    if ( event->get_event_type() == Gdk::Event::Type::KEY_RELEASE )
    {
        event_handled = on_key_released( event->get_keyval(),
                                         event->get_keycode(),
                                         event->get_consumed_modifiers() );
    }

    return event_handled;
}

//----------------------------------------------------------------------------

bool SearchDialog::on_key_released( unsigned const keyval,
                                    unsigned /* const keycode */,
                                    Gdk::ModifierType const state )
{
    // Making this table static relies on there only ever being one SearchDialog instance.

    static KeyMatch const key_matches[] =
    {
        { GDK_KEY_Return,   Gdk::ModifierType::NO_MODIFIER_MASK, [ & ]{ push_find_button(); } },
        { GDK_KEY_KP_Enter, Gdk::ModifierType::NO_MODIFIER_MASK, [ & ]{ push_find_button(); } },
        { GDK_KEY_Escape,   Gdk::ModifierType::NO_MODIFIER_MASK, [ & ]{ push_close_button(); } },
        {}
    };

    return match_key( "SearchDialog::on_key_released()", keyval, state, key_matches );
}

//----------------------------------------------------------------------------

void SearchDialog::push_find_button()
{
    if ( find_button.is_sensitive() )
    {
        find_button.activate();
    }
}

//----------------------------------------------------------------------------

void SearchDialog::push_close_button()
{
    if ( close_button.is_sensitive() )
    {
        close_button.activate();
    }
}

//============================================================================
