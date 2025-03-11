#include "MarkdownParser.hpp"
#include "TextBuffer.hpp"

#include <cstddef>  // std::size_t
#include <cstdint>  // std::uint8_t

constexpr std::size_t max_nesting = 16;

//============================================================================

MarkdownParser::MarkdownParser( mkd_extensions  const   markdown_extensions,
                                sd_callbacks    const&  parser_callbacks,
                                html_renderopt  const&  html_options )

: markdown_parser{ sd_markdown_new( markdown_extensions,
                                    max_nesting,
                                    &parser_callbacks,
                                    const_cast< html_renderopt* >( &html_options ) ) }
{
}

//----------------------------------------------------------------------------

MarkdownParser::~MarkdownParser()
{
    sd_markdown_free( markdown_parser );
}

//----------------------------------------------------------------------------

std::string MarkdownParser::render( std::string const& markdown_text ) const
{
    TextBuffer html_buffer;

    sd_markdown_render( html_buffer,
                        reinterpret_cast< std::uint8_t const* >( markdown_text.data() ),
                        markdown_text.size(),
                        markdown_parser );

    return html_buffer;
}

//============================================================================
