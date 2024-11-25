#ifndef INCLUDED_CMARK_PARSER_HPP
#define INCLUDED_CMARK_PARSER_HPP

#include <string>

class CMarkParser
{
public:

    static std::string convert_to_html( std::string const& markdown );
};

#endif // INCLUDED_CMARK_PARSER_HPP
