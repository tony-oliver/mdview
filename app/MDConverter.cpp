#include "MDConverter.hpp"

// DEBUG!!!
#include <iomanip>
#include <ostream>
#include <iostream>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

void append_string( char const* const data,
                    unsigned    const size,
                    void*       const userdata )
{
    static_cast< std::string* >( userdata )->append( data, size );
};

//----------------------------------------------------------------------------

void convert_align_attributes_in( std::string& html )
{
    // transform all obsolete alignment attributes into inline CSS
    // (attribute align="zzz" becomes attribute style="text-align: zzz")

    for ( ;; )
    {
        static std::string const pattern = " align=\"";
        static std::string const replacement = " style=\"text-align: ";

        auto const pos = html.find( pattern );
        if ( pos == std::string::npos ) break;

        html.replace( pos, pattern.length(), replacement );
    }
}

//----------------------------------------------------------------------------
} // close unnamed namespace
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
    std::string html;

    md_html( markdown.data(), markdown.size(),
             append_string, &html,
             parser_flags, render_flags );

    // The md4c module still generates align="zzz" attributes,
    // but these are now declared obsolete in HTML5 (in favour of
    // presentation-hint attributes using style="text-align: zzz").

    convert_align_attributes_in( html );

    return html;
}

//============================================================================
