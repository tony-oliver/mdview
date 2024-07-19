#ifndef INCLUDED_LIBTIDY_HPP
#define INCLUDED_LIBTIDY_HPP

#include "tidy.h"
#include "tidybuffio.h"

#include <string>
#include <ostream>

class LibTidy
{
    TidyDoc tidyDoc;
    std::ostream& logger;

public:

    LibTidy( std::ostream& logger );
    ~LibTidy();

    std::string tidyHtml( std::string const& untidyHtml ) const;
};

#endif // INCLUDED_LIBTIDY_HPP
