#include "MDConverter.hpp"

//============================================================================

MDConverter::MDConverter( unsigned const md4c_markdown_parser_flags,
                          unsigned const md4c_html_renderer_flags )
: parser_flags{ md4c_markdown_parser_flags }
, render_flags{ md4c_html_renderer_flags }
{
}

//----------------------------------------------------------------------------

std::string MDConverter::convert( std::string const& markdown ) const
{
    constexpr auto append_string = []( char const*  const data,
                                       unsigned     const size,
                                       void*        const userdata )
    {
        static_cast< std::string* >( userdata )->append( data, size );
    };

    std::string html;

    md_html( markdown.data(), markdown.size(),
             append_string, &html,
             parser_flags, render_flags );

    return html;
}

//============================================================================
