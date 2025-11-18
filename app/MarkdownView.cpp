#include "MarkdownView.hpp"
#include "MDConverter.hpp"
#include "HTMLTidier.hpp"

#include <fstream>          // std::ifstream{}
#include <istream>          // std::istream{}
#include <ostream>          // std::ostream{}
#include <iostream>         // std::cout, std::cerr
#include <iterator>         // std::istreambuf_iterator<>{}
#include <exception>        // std::exception{}
#include <system_error>     // std::system_error{}, std::generic_category()

//============================================================================
namespace { // unnamed
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

// Items to address:
//
// Font-size for <code> elements    (left alone in Eclipse; 'larger' in github)
// Font-colour for <code> elements  ('mediumblue' in Eclipse; left alone in github)
// Text-decoration for <a> elemnts  (underline always/on hover)

std::string make_css()
{
    return
    R"(
        code
        {
            color: mediumblue;
            // font-size: larger;
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
        
        a
        {
            color: mediumblue;
            text-decoration: none;
        }

        a:hover
        {
            cursor: grab;
            text-decoration: underline !important;
        } 
    )";
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MarkdownView::MarkdownView( std::ostream& logger,
                            std::string const& filename,
                            bool const dump_html,
                            bool const show_diagnostics )
: dump_html{ dump_html }
, show_diagnostics{ show_diagnostics }
, filename{ filename }
, watcher( logger )
{
    render(); // first-time render
}

//----------------------------------------------------------------------------

void MarkdownView::render()
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

        if ( !watcher.running() )
        {
            watcher.watchFile( filename, [ & ]{ render(); } );

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

void MarkdownView::postProcess( std::string& html )
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

//============================================================================
