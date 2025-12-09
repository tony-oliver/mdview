#include "MarkdownView.hpp"
#include "MDConverter.hpp"
#include "HTMLTidier.hpp"
#include "KeyMatch.hpp"

#include <fstream>          // std::ifstream{}
#include <istream>          // std::istream{}
#include <iomanip>          // std::quoted<>()
#include <ostream>          // std::ostream{}
#include <iostream>         // std::cout, std::cerr
#include <iterator>         // std::istreambuf_iterator<>{}
#include <algorithm>        // std::upper_bound<>()
#include <exception>        // std::exception{}
#include <system_error>     // std::system_error{}, std::generic_category()

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

enum MouseButton
{
    Primary     = GDK_BUTTON_PRIMARY,
    Secondary   = GDK_BUTTON_SECONDARY,
    Middle      = GDK_BUTTON_MIDDLE,
    GoBack      = 8,
    GoForward   = 9,
};

//----------------------------------------------------------------------------

// These are the allowable +/- zoom levels in Firefox.
constexpr double const zoom_levels[]
{
    0.30,
    0.50,
    0.67,
    0.80,
    0.90,
    1.00,
    1.10,
    1.20,
    1.33,
    1.50,
    1.70,
    2.00,
    2.40,
    3.00,
    4.00,
    5.00,
};

//----------------------------------------------------------------------------

std::string extract_file_contents( std::string const& filename )
{

    std::ifstream file( filename );
    if ( !file ) throw std::system_error( errno, std::generic_category(), filename );

    using Iterator = std::istreambuf_iterator< char >;
    return std::string( Iterator( file ), Iterator{} );
}

//----------------------------------------------------------------------------

void wrap_html( std::string& html, std::string const& tag )
{
    std::string const prefix = "\n<" + tag + ">\n";
    std::string const suffix = "\n</" + tag + ">\n";
    html.reserve( prefix.length() + html.length() + suffix.length() );
    html.insert( 0, prefix );
    html.append( suffix );
}

//----------------------------------------------------------------------------

std::string make_css()
{
    return
    R"(
        code
        {
            color: mediumblue;
            #font-size: larger;
        }

        pre
        {
            padding: 15px;
            background-color: whitesmoke;
        }

        h1, h2, h3, h4, h5, h6
        {
            border-bottom: 1px solid gainsboro;
        }

        hr
        {
            border: 0;
            height: 1px;
            background-color: gainsboro;
        }

        table, th, td
        {
            border: 1px solid gainsboro;
        }

        table
        {
            border-spacing: 0;
        }

        th, td
        {
            padding: 5px;
        }

        th
        {
            font-weight: bold;
        }

        tr:nth-child( even ) td
        {
            background-color: whitesmoke;
        }
        
        a
        {
            color: mediumblue;
            text-decoration: none;
        }

        a:hover
        {
            text-decoration: underline !important;
        } 
    )";
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MarkdownView::MarkdownView( Gtk::Window& parent,
                            std::ostream& logger,
                            std::string const& filename,
                            bool const dump_html,
                            bool const show_diagnostics )
: dump_html{ dump_html }
, show_diagnostics{ show_diagnostics }
, filename{ filename }
, file_watcher( logger )
, keypress_tracker{ Gtk::EventControllerKey::create() }
, click_tracker{ Gtk::GestureClick::create() }
, find_controller{ get_find_controller() }
, search_dialog( parent )
{
    add_controller( keypress_tracker );
    keypress_tracker->signal_key_pressed().connect( sigc::mem_fun( *this, &MarkdownView::on_key_pressed ), false );

    add_controller( click_tracker );
    click_tracker->signal_unpaired_release().connect( sigc::mem_fun( *this, &MarkdownView::on_unpaired_button_release ) );

    search_dialog.set_search_action( [ & ]{ on_search_requested(); } );

    load_root_document();
}

//----------------------------------------------------------------------------

void MarkdownView::load_root_document()
{
    std::string html;

    try
    {
        /*--------------------------------------------------*\
        |*  Extract the markdown text from the named file.  *|
        \*--------------------------------------------------*/

        auto const markdown = extract_file_contents( filename );

        /*--------------------------------------*\
        |*  Convert the markdown text to HTML.  *|
        \*--------------------------------------*/

        MDConverter const html_from_markdown( MD_DIALECT_GITHUB );
        html = html_from_markdown.convert( markdown );

        /*----------------------------------------------------------------------*\
        |*  Repeat this operation whenever the markdown file contents change.   *|
        \*----------------------------------------------------------------------*/

        if ( !file_watcher.running() )
        {
            file_watcher.watchFile( filename, [ & ]{ load_root_document(); } );

            file_watcher.start();
        }
    }
    catch ( std::exception const& e )
    {
        /*----------------------------------------------------------*\
        |*  Present the error message instead of converted markdown *|
        \*----------------------------------------------------------*/

        html = e.what();

        std::cerr << "* " << html << std::endl;

        wrap_html( html, "code" );
        wrap_html( html, "pre" );
    }

    /*------------------------------------------------------------------*\
    |*  Add missing elements to the HTML and write out (if requested)   *|
    \*------------------------------------------------------------------*/

    post_process_html( html );

    if ( dump_html )
    {
        std::cout << html << std::flush;
    }

    /*----------------------------------------------------------*\
    |*  Finally, display the converted HTML in its GUI widget   *|
    \*----------------------------------------------------------*/

    load_html( html );
}

//----------------------------------------------------------------------------

void MarkdownView::post_process_html( std::string& html )
{
    auto style = make_css();
    wrap_html( style, "style" );
    html.insert( 0, style );

    HTMLTidier html_tidier;
    html = html_tidier.tidyupHTML( html );

    if ( show_diagnostics )
    {
        auto const diagnostics = html_tidier.getDiagnostics();

        if ( !diagnostics.empty() )
        {
            std::cerr << std::endl;
            std::cerr << "LIBTIDY DIAGNOSTICS:" << std::endl;
            std::cerr << diagnostics << std::endl;
        }
    }
}

//----------------------------------------------------------------------------

void MarkdownView::launch_search_dialog()
{
    search_dialog.set_visible( true );  // show the search-dialog
}

//----------------------------------------------------------------------------

void MarkdownView::zoom_in()
{
    std::clog << "MarkdownView::zoom_in()" << std::endl;

    auto const current_level = get_zoom_level();
    std::clog << "current_level = " << current_level << std::endl;

    auto p = std::ranges::find( zoom_levels, current_level );

    if ( ++p != std::cend( zoom_levels ) )
    {
        set_zoom_level( *p );
    }
}

//----------------------------------------------------------------------------

void MarkdownView::zoom_out()
{
    auto const current_level = get_zoom_level();
    std::clog << "current_level = " << current_level << std::endl;

    auto p = std::ranges::find( zoom_levels, current_level );

    if ( p != std::cbegin( zoom_levels ) )
    {
        set_zoom_level( *--p );
    }
}

//----------------------------------------------------------------------------

void MarkdownView::on_search_requested()
{
    search_dialog.set_visible( false );

    find_controller.search( search_dialog.get_search_text(),
                            search_dialog.get_find_options() );
}

//----------------------------------------------------------------------------

bool MarkdownView::on_key_pressed( unsigned const keyval, unsigned /* keycode */, Gdk::ModifierType const state )
{
    // Making this table static relies on there only ever being one MarkdownView instance.

    constexpr auto Unmodified = Gdk::ModifierType::NO_MODIFIER_MASK;
    constexpr auto Shift = Gdk::ModifierType::SHIFT_MASK;
    constexpr auto Ctrl = Gdk::ModifierType::CONTROL_MASK;
    constexpr auto Alt = Gdk::ModifierType::ALT_MASK;

    static KeyMatch const key_matches[] =
    {
        { GDK_KEY_f,            Ctrl,           [ & ]{ launch_search_dialog();              } },
        { GDK_KEY_Escape,       Unmodified,     [ & ]{ find_controller.search_finish();     } },

        { GDK_KEY_F3,           Unmodified,     [ & ]{ find_controller.search_next();       } },
        { GDK_KEY_F3,           Shift,          [ & ]{ find_controller.search_previous();   } },

        { GDK_KEY_n,            Unmodified,     [ & ]{ find_controller.search_next();       } },
        { GDK_KEY_N,            Shift,          [ & ]{ find_controller.search_previous();   } },

        { GDK_KEY_n,            Alt,            [ & ]{ find_controller.search_next();       } },
        { GDK_KEY_N,            Alt | Shift,    [ & ]{ find_controller.search_previous();   } },

        { GDK_KEY_Left,         Alt,            [ & ]{ go_back();                           } },
        { GDK_KEY_Right,        Alt,            [ & ]{ go_forward();                        } },

        { GDK_KEY_equal,        Ctrl,           [ & ]{ zoom_in();                           } },
        { GDK_KEY_plus,         Shift | Ctrl,   [ & ]{ zoom_in();                           } },
        { GDK_KEY_KP_Add,       Ctrl,           [ & ]{ zoom_in();                           } },

        { GDK_KEY_minus,        Ctrl,           [ & ]{ zoom_out();                          } },
        { GDK_KEY_underscore,   Shift | Ctrl,   [ & ]{ zoom_out();                          } },
        { GDK_KEY_KP_Subtract,  Ctrl,           [ & ]{ zoom_out();                          } },

        {}
    };

    return match_key( "MarkdownView::on_key_pressed()", keyval, state, key_matches );
}


//----------------------------------------------------------------------------

void MarkdownView::on_unpaired_button_release( [[maybe_unused]] double const x,
                                               [[maybe_unused]] double const y,
                                               unsigned const button,
                                               [[maybe_unused]] Gdk::EventSequence* const sequence )
{
    switch ( button )
    {
    case MouseButton::GoBack:       go_back();      break;
    case MouseButton::GoForward:    go_forward();   break;
    }
}

//============================================================================
