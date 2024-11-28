#ifndef INCLUDED_MARKDOWN_CONVERTER_HPP
#define INCLUDED_MARKDOWN_CONVERTER_HPP

#include <cmark.h>

#include <string>

class MarkdownConverter
{
    cmark_node* const root = nullptr;

public:

    class Exception; // differentiable from other std::exceptions

    explicit MarkdownConverter( std::string const& markdown );

    ~MarkdownConverter();

    std::string convert_to_xml() const;
    std::string convert_to_html() const;
    std::string convert_to_latex() const;
    std::string convert_to_manpage() const;
    std::string convert_to_markdown() const;

private:

    void check_returned_cmark_ptr( void const* ptr, std::string const& operation ) const;
};

#endif // INCLUDED_MARKDOWN_CONVERTER_HPP
