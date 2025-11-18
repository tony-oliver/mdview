#ifndef INCLUDED_HTML_TIDIER_HPP
#define INCLUDED_HTML_TIDIER_HPP

#include "tidy.h"
#include "tidybuffio.h"

#include <string>

class HTMLTidier
{
    // The handle referencing this instance of libtidy:

    TidyDoc const html_tidier;

    // The concrete text-buffer class used here:

    struct Buffer: TidyBuffer
    {
        Buffer();
        ~Buffer();

        void clear();
        operator std::string() const;
    };

public:

    HTMLTidier();
    ~HTMLTidier();

    // Encapsulated libtidy primitives:

    bool setBooleanOption( TidyOptionId option_id, Bool new_value );
    int setIntegerOption(  TidyOptionId option_id,  int new_value );

    int saveErrorsToBuffer( Buffer& buffer );
    int saveOutputToBuffer( Buffer& buffer );

    int ingestContent( std::string const& content );
    int cleanAndRepair();

    // High-level operations:

    std::string tidyupHTML( std::string const& html );
    std::string getDiagnostics() const;

private:

    Buffer output_buffer;
    Buffer diagnostics_buffer;
};

#endif // INCLUDED_HTML_TIDIER_HPP
