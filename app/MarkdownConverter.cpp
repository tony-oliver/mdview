#include "MarkdownConverter.hpp"

#include <array>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <stdexcept>
#include <system_error>

#include <ostream>
#include <iostream>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr auto options{ CMARK_OPT_DEFAULT };
constexpr auto width{ 0 };

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

// The cmark-specific esception type (which behave just like std::runtime_error)
// to facilitate CMarkParser-specific exception-catching.

class MarkdownConverter::Exception: public std::runtime_error
{
    using std::runtime_error::runtime_error; // inherit the constructor(s)
};

//----------------------------------------------------------------------------

MarkdownConverter::MarkdownConverter( std::string const& markdown )
: root{ cmark_parse_document( markdown.data(), markdown.size(), options ) }
{
    check_returned_cmark_ptr( root, "parse_document" );
}

//----------------------------------------------------------------------------

MarkdownConverter::~MarkdownConverter()
{
    cmark_node_free( root );
}

//----------------------------------------------------------------------------

std::string MarkdownConverter::convert_to_xml() const
{
    auto const rendered{ cmark_render_xml( root, options ) };
    check_returned_cmark_ptr( rendered, "render_xml" );
    std::string const result( rendered );
    std::free( rendered );
    return result;
}

//----------------------------------------------------------------------------

std::string MarkdownConverter::convert_to_html() const
{
    auto const rendered{ cmark_render_html( root, options ) };
    check_returned_cmark_ptr( rendered, "render_html" );
    std::string const result( rendered );
    std::free( rendered );
    return result;
}

//----------------------------------------------------------------------------

std::string MarkdownConverter::convert_to_latex() const
{
    auto const rendered{ cmark_render_latex( root, options, width ) };
    check_returned_cmark_ptr( rendered, "render_latex" );
    std::string const result( rendered );
    std::free( rendered );
    return result;
}

//----------------------------------------------------------------------------

std::string MarkdownConverter::convert_to_manpage() const
{
    auto const rendered{ cmark_render_man( root, options, width ) };
    check_returned_cmark_ptr( rendered, "render_man" );
    std::string const result( rendered );
    std::free( rendered );
    return result;
}

//----------------------------------------------------------------------------

std::string MarkdownConverter::convert_to_markdown() const
{
    auto const rendered{ cmark_render_commonmark( root, options, width ) };
    check_returned_cmark_ptr( rendered, "render_commonmark" );
    std::string const result( rendered );
    std::free( rendered );
    return result;
}

//----------------------------------------------------------------------------

void MarkdownConverter::check_returned_cmark_ptr( void const* const ptr, std::string const& operation ) const
{
    if ( ptr == nullptr )
    {
        throw Exception( "Internal error: cmark_" + operation + "() returned nullptr." );
    }
}

//============================================================================
