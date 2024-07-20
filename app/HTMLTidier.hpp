#ifndef INCLUDED_HTML_TIDIER_HPP
#define INCLUDED_HTML_TIDIER_HPP

#include "tidy.h"
#include "tidybuffio.h"

#include <string>
#include <ostream>

class HTMLTidier
{
    TidyDoc tidyDoc;
    std::ostream& logger;

    struct Buffer: TidyBuffer
    {
        Buffer();
        ~Buffer();
    };
public:

    HTMLTidier( std::ostream& logger );
    ~HTMLTidier();

    // Low-level operations

    int setErrorBuffer( Buffer& buffer );
    int setIntegerOption(  TidyOptionId optionID, int newValue );
    bool setBooleanOption( TidyOptionId optionID, Bool newValue );
    int parseString( std::string const& string );
    int cleanAndRepair();
    int saveToBuffer( Buffer& buffer );

    // High-level operations

    std::string tidyup( std::string const& untidyHtml );
};

#endif // INCLUDED_HTML_TIDIER_HPP
