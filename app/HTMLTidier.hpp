#ifndef INCLUDED_HTML_TIDIER_HPP
#define INCLUDED_HTML_TIDIER_HPP

#include "tidy.h"
#include "tidybuffio.h"

#include <string>

class HTMLTidier
{
    TidyDoc tidy_doc;

    struct Buffer: TidyBuffer
    {
        Buffer();
        ~Buffer();
    };

    Buffer output_buffer;
    Buffer diagnostics_buffer;

public:

    HTMLTidier();
    ~HTMLTidier();

    std::string tidyupHTML( std::string const& untidyHtml );
    std::string getDiagnostics() const;

private:

    int setErrorBuffer( Buffer& buffer );
    int setIntegerOption(  TidyOptionId option_id, int new_value );
    bool setBooleanOption( TidyOptionId option_id, Bool new_value );

    int ingestContent( std::string const& content );
    int cleanAndRepair();
    int saveToBuffer( Buffer& buffer );

    std::string contentsOfBuffer( Buffer const& buffer ) const;
};

#endif // INCLUDED_HTML_TIDIER_HPP
