#include "MainWindow.hpp"

#include <gtkmm/enums.h>

#include "cmark.h"

#include <cerrno>           // errno
#include <string>           // std::to_string<>()
#include <cstring>          // std::strerror()
#include <fstream>          // std::ifstream{}
#include <iostream>         // std::cerr{}
#include <iterator>         // std::istreambuf_iterator{}
#include <exception>        // std::exception{}
#include <stdexcept>        // std::runtime_error{}
#include <initializer_list> // std::initializer_list<>{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string progname = "mdview";

void check_just_one_arg_in( MainWindow::Arguments const& args )
{
    if ( args.size() != 1 )
    {
        throw std::runtime_error( "This program must be invoked with a single filename argument." );
    }
}

void check_file_opened_ok( std::ifstream const& file, std::string const& filename )
{
    if ( !file.is_open() )
    {
        throw std::runtime_error( filename + ": error #" + std::to_string( errno ) + " (" + std::strerror( errno ) + ")" );
    }
}

void check_converted_to_html_ok( char const* const converted_text )
{
    if ( !converted_text )
    {
        throw std::runtime_error( "Internal error: cmark_markdown_to_html() returned nullptr." );
    }
}

void wrap_html( std::string& html, std::initializer_list< std::string > const& elements )
{
    std::string prefix, suffix;

    for ( auto const& element: elements )
    {
        prefix.append( "<" + element + ">\n" );
        suffix.insert( 0, "</" + element + ">\n" );
    }

    html.reserve( prefix.length() + html.length() + suffix.length() );
    html.insert( 0, prefix );
    html.append( suffix );
}

std::string make_css()
{
    return R"(code { color: mediumblue; }
pre { padding: 15px; background-color: whitesmoke; }
h1, h2, h3, h4 { border-bottom: 1px solid gainsboro; }
)";
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Arguments const& args )
{
    std::string html;

    try
    {
        check_just_one_arg_in( args );
        auto const filename{ args.front() };

        std::ifstream markdownFile( filename );
        check_file_opened_ok( markdownFile, filename );

        set_title( progname + " - " + filename );

        using Iterator = std::istreambuf_iterator< char >;
        std::string const markdownText{ Iterator{ markdownFile }, Iterator{} };

        auto const converted_text = cmark_markdown_to_html( markdownText.data(), markdownText.size(), 0 );
        check_converted_to_html_ok( converted_text );

        html = converted_text;
        std::free( converted_text );
    }
    catch ( std::exception const& e )
    {
        html = e.what();
        std::cerr << '\n' << html << '\n' << std::endl;
        wrap_html( html, { "pre", "code" } );
    }

    wrap_html( html, { "body" } );

    auto style = make_css();
    wrap_html( style, { "head", "style" } );

    html.insert( 0, style );
    wrap_html( html, { "html" } );

    std::cout << html << std::endl;

    webView.load_html( html );
    scroller.set_child( webView );
    scroller.set_policy( Gtk::PolicyType::NEVER, Gtk::PolicyType::ALWAYS );
    set_child( scroller );

    set_default_size( 1024, 768 );
}
