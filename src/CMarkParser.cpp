#include "CMarkParser.hpp"

#include <cmark.h>          // cmark_markdown_to_html()

#include <stdexcept>

std::string CMarkParser::convert_to_html( std::string const& markdown )
{
    constexpr auto options = CMARK_OPT_DEFAULT;

    auto const html_ptr{ cmark_markdown_to_html( markdown.data(), markdown.size(), options ) };

    if ( html_ptr == nullptr )
    {
        throw std::runtime_error( "Internal error: cmark_markdown_to_html() returned nullptr." );
    }

    std::string const html( html_ptr );
    free( html_ptr );
    return html;
}
