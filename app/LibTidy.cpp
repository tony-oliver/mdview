#include "LibTidy.hpp"

#include <cstdlib>

#include <iomanip>
#include <ostream>

LibTidy::LibTidy( std::ostream& logger )
: tidyDoc{ tidyCreate() }
, logger{ logger }
{
}

LibTidy::~LibTidy()
{
    tidyRelease( tidyDoc );
}

std::string LibTidy::tidyHtml( std::string const& html ) const
{
    std::string tidiedHtml;

    TidyBuffer output{};

    tidyParseString(    tidyDoc, html.c_str() );            // Parse the input
    tidyOptSetBool(     tidyDoc, TidyHtmlOut, yes );        // Convert to HTML
    tidyOptSetBool(     tidyDoc, TidyForceOutput, yes );    // Even if there were errors
    tidyOptSetInt(      tidyDoc, TidyIndentAttributes, yes );
    tidyOptSetInt(      tidyDoc, TidyIndentContent, yes );
    tidyOptSetInt(      tidyDoc, TidyIndentCdata, yes );
    tidyOptSetInt(      tidyDoc, TidyIndentSpaces, 2 );
    tidyOptSetInt(      tidyDoc, TidyWrapLen, 132 );
    tidyCleanAndRepair( tidyDoc );                          // Tidy it up!
    tidySaveBuffer(     tidyDoc, &output );                 // Pretty Print to output buffer

    auto const outstr = reinterpret_cast< char const* >( output.bp );

    if ( outstr != nullptr ) tidiedHtml = outstr;

    tidyBufFree( &output );

    return tidiedHtml;
}
