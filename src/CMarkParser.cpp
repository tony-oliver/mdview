#include "CMarkParser.hpp"

#include <stdexcept>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr auto options{ CMARK_OPT_DEFAULT };
constexpr auto width{ 0 };

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

CMarkParser::CMarkParser( std::string const& document )
: root{ cmark_parse_document( document.data(), document.size(), options ) }
{
    if ( root == nullptr ) throw_cmark_error( "parse_document" );
}

CMarkParser::~CMarkParser()
{
    cmark_node_free( root );
}

std::string CMarkParser::convert_to_xml() const
{
    auto const rendered{ cmark_render_xml( root, options ) };
    if ( rendered == nullptr ) throw_cmark_error( "render_xml" );
    std::string const result( rendered );
    free( rendered );
    return result;
}

std::string CMarkParser::convert_to_html() const
{
    auto const rendered{ cmark_render_html( root, options ) };
    if ( rendered == nullptr ) throw_cmark_error( "render_html" );
    std::string const result( rendered );
    free( rendered );
    return result;
}

std::string CMarkParser::convert_to_latex() const
{
    auto const rendered{ cmark_render_latex( root, options, width ) };
    if ( rendered == nullptr ) throw_cmark_error( "render_latex" );
    std::string const result( rendered );
    free( rendered );
    return result;
}

std::string CMarkParser::convert_to_manpage() const
{
    auto const rendered{ cmark_render_man( root, options, width ) };
    if ( rendered == nullptr ) throw_cmark_error( "render_man" );
    std::string const result( rendered );
    free( rendered );
    return result;
}

std::string CMarkParser::convert_to_markdown() const
{
    auto const rendered{ cmark_render_commonmark( root, options, width ) };
    if ( rendered == nullptr ) throw_cmark_error( "render_commonmark" );
    std::string const result( rendered );
    free( rendered );
    return result;
}

void CMarkParser::throw_cmark_error( std::string const& operation ) const
{
    throw std::runtime_error( "Internal error: cmark_" + operation + "() returned nullptr." );
}
