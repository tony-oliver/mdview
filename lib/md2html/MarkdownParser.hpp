#ifndef INCLUDED_MARKDOWN_PARSER_HPP
#define INCLUDED_MARKDOWN_PARSER_HPP

#include "markdown.h"   // mkd_extensions, sd_callbacks{}
#include "html.h"       // html_renderopt

#include <string>

//============================================================================

class MarkdownParser
{
    sd_markdown* const markdown_parser;

public:

    MarkdownParser( mkd_extensions          markdown_extensions,
                    sd_callbacks    const&  parser_callbacks,
                    html_renderopt  const&  html_options );

    ~MarkdownParser();

    std::string render( std::string const& markdown_text ) const;
};

//============================================================================

#endif // INCLUDED_MARKDOWN_PARSER_HPP
