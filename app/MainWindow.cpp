#include "MainWindow.hpp"
#include "MarkdownConverter.hpp"
#include "HTMLRenderer.hpp"
#include "HTMLTidier.hpp"

#include <md4c-html.h>

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
h1, h2, h3, h4 { border-bottom: 1px solid gainsboro; }
hr { background-color: gainsboro; height: 1px; border: 0; })";
}

//----------------------------------------------------------------------------

constexpr unsigned md4c_parser_flags = MD_DIALECT_GITHUB;
constexpr unsigned md4c_render_flags = 0; // MD_HTML_FLAG_xxx

//----------------------------------------------------------------------------

void append_html( char const* const data, unsigned const size, void* const userdata )
{
    auto html = static_cast< std::string* >( userdata );

    html->append( data, size );
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

    watcher.start();

    displayMarkdownFile();
}

//----------------------------------------------------------------------------

void MainWindow::displayMarkdownFile()
{
    std::cout << "*** displayMarkdownFile() ***" << std::endl;
    std::string html;

    try
    {
        std::ifstream file( filename );
        if ( !file ) throw std::system_error( errno, std::generic_category(), filename );

        using Iterator = std::istreambuf_iterator< char >;
        std::string const markdown( Iterator( file ), Iterator{} );

        switch ( options.get_converter() )
        {
        case CMark:
            logger << "Using cmark converter." << std::endl;
            html = MarkdownConverter( markdown ).convert_to_html();
            break;

        case Sundown:
            logger << "Using sundown converter." << std::endl;
            html = HTMLRenderer{}.render( markdown );
            break;

        case MD4C:
            logger << "Using md4c converter." << std::endl;
            md_html( markdown.data(), markdown.size(), append_html, &html, md4c_parser_flags, md4c_render_flags );
            break;
        }

        if ( !watcher.isWatching( filename ) )
        {
            watcher.watchFile( filename, [ this ]{ displayMarkdownFile(); } );
        }
    }
    catch ( std::exception const& e )
    {
        html = e.what();

        std::cerr << "* " << html << std::endl;

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
