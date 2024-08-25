#include "MainWindow.hpp"
#include <gtkmm/enums.h>

#include "cmark.h"

#include <cerrno>           // errno
#include <memory>           // std::unique_ptr<>{}
#include <string>           // std::to_string<>()
#include <cstdlib>          // std::free()
#include <cstring>          // std::strerror()
#include <fstream>          // std::ifstream{}
#include <iostream>         // std::cerr{}
#include <iterator>         // std::istreambuf_iterator{}
#include <exception>        // std::exception{}
#include <stdexcept>        // std::runtime_error{}
#include <initializer_list> // std::initializer_list<>{}
#include "HTMLTidier.hpp"

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

void check_file_opened_ok( std::ifstream const& file, std::string const& filename )
{
    if ( !file.is_open() )
    {
        throw std::system_error( errno, std::generic_category(), filename );
    }
}

void check_converted_to_html_ok( char const* const converted_text_ptr )
{
    if ( !converted_text_ptr )
    {
        throw std::runtime_error( "Internal error: cmark_markdown_to_html() returned nullptr." );
    }
}

void wrap_html( std::string& html, std::string const& tag )
{
    std::string const prefix = "\n<" + tag + ">\n";
    std::string const suffix = "\n</" + tag + ">\n";
    html.reserve( prefix.length() + html.length() + suffix.length() );
    html.insert( 0, prefix );
    html.append( suffix );
}

std::string make_css()
{
return R"(code { color: mediumblue; }
pre { padding: 15px; background-color: whitesmoke; }
h1, h2, h3, h4 { border-bottom: 1px solid gainsboro; })";
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Options const& options )
: signalHandler( { SIGINT, SIGTERM, SIGPWR, SIGHUP } )
, options{ options }
, filename{ options.get_filename() }
, logger{ options.get_logger() }
, makeThreadSafe( logger, options.get_use_colour() )
, watcher( logger )
{
    signalHandler.registerAction( [ & ]{ close(); } );

    scroller.set_policy( Gtk::PolicyType::NEVER, Gtk::PolicyType::ALWAYS );
    scroller.set_child( webViewWidget );
    set_child( scroller );

    auto title = appname;
    if ( !filename.empty() ) title += " - " + filename;
    set_title( title );

    set_default_size( 1024, 768 );

    displayMarkdownFile();
}

void MainWindow::postProcess( std::string& html )
{
    auto style = make_css();
    wrap_html( style, "style" );

    HTMLTidier tidier( logger );
    html = tidier.tidyupHTML( style + html );

    if ( options.get_show_diagnostics() )
    {
        std::cout << "LIBTIDY DIAGNOSTICS:\n" << tidier.getDiagnostics() << std::endl;
    }
}

void MainWindow::displayMarkdownFile()
{
    std::string html;

    try
    {
        std::ifstream markdownFile( filename );
        check_file_opened_ok( markdownFile, filename );

        if ( !watcher.isWatching( filename ) )
        {
            watcher.watchFile( filename, [ this ]{ displayMarkdownFile(); } );
            watcher.start();
        }

        using Iterator = std::istreambuf_iterator< char >;
        std::string const markdownText{ Iterator{ markdownFile }, Iterator{} };

        auto const html_ptr{ cmark_markdown_to_html( markdownText.data(), markdownText.size(), 0 ) };
        check_converted_to_html_ok( html_ptr );
        html.assign( html_ptr );
        free( html_ptr );
    }
    catch ( std::exception const& e )
    {
        html = e.what();
        std::cerr << "\n* " << html << '\n' << std::endl;

        wrap_html( html, "code" );
        wrap_html( html, "pre" );
    }

    postProcess( html );

    if ( options.get_dump_html() )
    {
        std::cout << html << std::flush;
    }

    webViewWidget.load_html( html );
}
