#ifndef INCLUDED_HTML_TIDIER_HPP
#define INCLUDED_HTML_TIDIER_HPP

#include "tidy.h"
#include "tidybuffio.h"

#include <string>
#include <ostream>

class HTMLTidier
{
    TidyDoc tidyDoc;

    struct Buffer: TidyBuffer
    {
        Buffer();
        ~Buffer();
    };

    Buffer output;
    Buffer errbuf;

public:

    HTMLTidier();
    ~HTMLTidier();

    std::string tidyupHTML( std::string const& untidyHtml );
    std::string getDiagnostics() const;

private:

    int setErrorBuffer( Buffer& buffer );
    int setIntegerOption(  TidyOptionId optionID, int newValue );
    bool setBooleanOption( TidyOptionId optionID, Bool newValue );

    int ingestMarkdown( std::string const& string );
    int cleanAndRepair();
    int saveToBuffer( Buffer& buffer );
};

#endif // INCLUDED_HTML_TIDIER_HPP
