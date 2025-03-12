#include "HTMLRenderer.hpp"
#include "MarkdownParser.hpp"

//============================================================================

HTMLRenderer::HTMLRenderer( html_render_mode const render_flags )
{
    // populate the callbacks and options structures with HTML-rendering functions and flags
    sdhtml_renderer( &html_callbacks, &html_options, render_flags );
}

//----------------------------------------------------------------------------

std::string HTMLRenderer::render( std::string const& markdown_text )
{
    constexpr auto all_extensions = static_cast< mkd_extensions >( ~MKDEXT_NO_INTRA_EMPHASIS );

    // create a markdown-parser that uses the HTML-gneration callbacks and options
    MarkdownParser markdown_parser( all_extensions, html_callbacks, html_options );

    return markdown_parser.render( markdown_text );
};

//============================================================================
