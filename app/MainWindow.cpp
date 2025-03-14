#include "MainWindow.hpp"
#include "MDConverter.hpp"
#include "HTMLTidier.hpp"

#include <gtkmm/version.h>

#include <cerrno>           // errno
#include <string>           // std::string{}
#include <fstream>          // std::ifstream{}
#include <iomanip>          // std::quoted<>()
#include <iostream>         // std::cerr{}
#include <iterator>         // std::istreambuf_iterator{}
#include <exception>        // std::exception{}
#include <system_error>     // std::system_error{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

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
        code { color: mediumblue; }
        pre { padding: 15px; background-color: whitesmoke; }
        h1, h2, h3, h4, h5, h6 { border-bottom: 1px solid gainsboro; }
        hr { background-color: gainsboro; height: 1px; border: 0; }
        table, th, td { border: 1px solid gainsboro; }
        table { border-spacing: 0; }
        th, td { padding: 5px; }
        th { font-weight: bold; }
        a { color: mediumblue; text-decoration: none; }
    )";
}

//----------------------------------------------------------------------------

void log_components_to( std::ostream& logger )
{
    logger << "-------------------------------" << std::endl;

    logger << "GTKmm header version = "
           << GTKMM_MAJOR_VERSION << "."
           << GTKMM_MINOR_VERSION << "."
           << GTKMM_MICRO_VERSION << std::endl;

    // Desired: GTKmm library version

    logger << "WebKit header version = "
           << WEBKIT_MAJOR_VERSION << "."
           << WEBKIT_MINOR_VERSION << "."
           << WEBKIT_MICRO_VERSION << std::endl;

    logger << "WebKit library version = "
            << webkit_get_major_version() << "."
            << webkit_get_minor_version() << "."
            << webkit_get_micro_version() << std::endl;

    // Desired: MD4C header version
    // Desired: MD4C library version
    // Desired: LibTidy header version

    logger << "LibTidy library version = "
           << tidyLibraryVersion() << std::endl;

    logger << "-------------------------------" << std::endl;
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

    auto title = appname;
    if ( !filename.empty() ) title += " - " + filename;

    set_title( title );
    set_child( webView );
    set_default_size( 1200, 800 );

    log_components_to( logger );

    displayMarkdownFile();
}

//----------------------------------------------------------------------------

void MainWindow::displayMarkdownFile()
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

        html = MDConverter{}.convert( markdown );

        /*----------------------------------------------------------------------*\
        |*  Repeat this operation whenever the markdown file contents change.   *|
        \*----------------------------------------------------------------------*/

        if ( !watcher.running() )
        {
            watcher.watchFile( filename, [ this ]{ displayMarkdownFile(); } );

            watcher.start();
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

    postProcess( html );

    if ( options.get_dump_html() )
    {
        std::cout << html << std::flush;
    }

    /*----------------------------------------------------------*\
    |*  Finally, display the converted HTML in its GUI widget   *|
    \*----------------------------------------------------------*/

    webView.load_html( html );
}

//----------------------------------------------------------------------------

void MainWindow::postProcess( std::string& html )
{
    auto style = make_css();
    wrap_html( style, "style" );
    html.insert( 0, style );

    HTMLTidier tidier( logger );
    html = tidier.tidyupHTML( html );

    if ( options.get_show_diagnostics() )
    {
        std::cout << "LIBTIDY DIAGNOSTICS:\n" << tidier.getDiagnostics() << std::endl;
    }
}

//============================================================================
