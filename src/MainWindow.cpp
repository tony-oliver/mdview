#include "MainWindow.hpp"

#include "md4c-html.h"

#include <cerrno>       // errno
#include <string>       // std::to_string<>()
#include <cstring>      // std::strerror()
#include <fstream>      // std::ifstream{}
#include <iterator>     // std::istreambuf_iterator{}
#include <exception>    // std::exception{}
#include <stdexcept>    // std::runtime_error{}

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string progname = "mdview";

void append_to_string( char const* const html_data, unsigned const html_size, void* const html_ptr )
{
    static_cast< std::string* >( html_ptr )->append( html_data, html_size );
}

constexpr unsigned parser_flags = 0
| MD_FLAG_COLLAPSEWHITESPACE
| MD_FLAG_TABLES
| MD_FLAG_TASKLISTS
| MD_FLAG_STRIKETHROUGH
| MD_FLAG_PERMISSIVEURLAUTOLINKS
| MD_FLAG_PERMISSIVEEMAILAUTOLINKS
| MD_FLAG_PERMISSIVEWWWAUTOLINKS
| MD_FLAG_LATEXMATHSPANS
| MD_FLAG_WIKILINKS
| MD_FLAG_UNDERLINE
;

constexpr unsigned renderer_flags = 0
| MD_HTML_FLAG_DEBUG
| MD_HTML_FLAG_VERBATIM_ENTITIES
| MD_HTML_FLAG_SKIP_UTF8_BOM
| MD_HTML_FLAG_XHTML
;

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Arguments const& args )
{
    std::string htmlContent;

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

        int const md_html_result = md_html( markdownText.data(),
                                            markdownText.size(),
                                            append_to_string,
                                            &htmlContent,
                                            parser_flags,
                                            renderer_flags );

        if ( md_html_result != 0 )
        {
            throw std::runtime_error( "Internal error: md_html() returned " + std::to_string( md_html_result ) );
        }
    }
    catch ( std::exception const& e )
    {
        htmlContent = e.what();

        htmlContent.insert( 0, "<pre><code>" );
        htmlContent.append( "</code></pre>" );
    }

    htmlContent.insert( 0, "<html><body>\n" );
    htmlContent.append( "\n</body></html>" );

    webView.load_html( htmlContent );
    set_child( webView );
    set_default_size( 1024, 768 );
}
