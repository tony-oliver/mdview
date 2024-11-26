#ifndef INCLUDED_CMARK_PARSER_HPP
#define INCLUDED_CMARK_PARSER_HPP

#include <cmark.h>

#include <string>

class CMarkParser
{
    cmark_node* const root;

public:

    explicit CMarkParser( std::string const& document );
    ~CMarkParser();

    std::string convert_to_xml() const;
    std::string convert_to_html() const;
    std::string convert_to_latex() const;
    std::string convert_to_manpage() const;
    std::string convert_to_markdown() const;

private:

    void throw_cmark_error( std::string const& operation ) const;
};

#endif // INCLUDED_CMARK_PARSER_HPP
