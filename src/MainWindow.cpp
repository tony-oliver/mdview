#include "MainWindow.hpp"
#include "HTMLTidier.hpp"
#include "CMarkParser.hpp"

#include <cerrno>           // errno
#include <cstdlib>          // std::free()
#include <fstream>          // std::ifstream{}
#include <iostream>         // std::cerr{}
#include <iterator>         // std::istreambuf_iterator{}
#include <exception>        // std::exception{}
#include <stdexcept>        // std::runtime_error{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

//----------------------------------------------------------------------------

void check_file_opened_ok( std::ifstream const& file, std::string const& filename )
{
    if ( !file.is_open() )
    {
        throw std::system_error( errno, std::generic_category(), filename );
    }
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

    set_child( webView );

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
        html = CMarkParser::convert_to_html( markdownText );
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

    webView.load_html( html );
}
