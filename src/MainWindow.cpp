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

void wrap_html( std::string& html, std::initializer_list< std::string > const& elements )
{
    std::string prefix, suffix;

    for ( auto const& element: elements )
    {
        prefix.append( "<" + element + ">" );
        suffix.insert( 0, "</" + element + ">" );
    }

    html.reserve( prefix.length() + html.length() + suffix.length() );
    html.insert( 0, prefix );
    html.append( suffix );
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Arguments const& args )
{
    std::string html;

    try
    {
        if ( args.size() != 1 )
        {
            throw std::runtime_error( "This program must be invoked with a single filename argument." );
        }

        auto const filename{ args.front() };
        std::ifstream markdownFile( filename );

        if ( !markdownFile.is_open() )
        {
            throw std::runtime_error( filename + ": error #" + std::to_string( errno ) + " (" + std::strerror( errno ) + ")" );
        }

        set_title( progname + " - " + filename );

        using Iterator = std::istreambuf_iterator< char >;
        std::string const markdownText{ Iterator{ markdownFile }, Iterator{} };
        auto const result = cmark_markdown_to_html( markdownText.data(), markdownText.size(), 0 );

        if ( result == nullptr )
        {
            throw std::runtime_error( "Internal error: cmark_markdown_to_html() returned nullptr." );
        }

        html = result;
        std::free( result );
    }
    catch ( std::exception const& e )
    {
        html = e.what();
        std::cerr << '\n' << html << '\n' << std::endl;
        wrap_html( html, { "pre", "code" } );
    }

    wrap_html( html, { "html", "body" } );

    webView.load_html( html );
    scroller.set_child( webView );
    scroller.set_policy( Gtk::PolicyType::NEVER, Gtk::PolicyType::ALWAYS );
    set_child( scroller );

    set_default_size( 1024, 768 );
}
