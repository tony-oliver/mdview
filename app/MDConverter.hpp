#ifndef INCLUDED_MD_CONVERTER_HPP
#define INCLUDED_MD_CONVERTER_HPP

#include <md4c-html.h>

#include <string>

class MDConverter
{
    unsigned const parser_flags;
    unsigned const render_flags;

public:

    MDConverter( unsigned md4c_markdown_parser_flags = MD_DIALECT_GITHUB,
                 unsigned md4c_html_renderer_flags = {} );

    std::string convert( std::string const& markdown ) const;
};

#endif // INCLUDED_MD_CONVERTER_HPP
