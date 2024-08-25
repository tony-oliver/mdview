#include "HTMLTidier.hpp"

#include <cstdlib>

#include <iomanip>
#include <ostream>

HTMLTidier::Buffer::Buffer()
: TidyBuffer{}
{
}

HTMLTidier::Buffer::~Buffer()
{
    tidyBufFree( this );
}

HTMLTidier::HTMLTidier( std::ostream& logger )
: tidyDoc{ tidyCreate() }
, logger{ logger }
{
}

HTMLTidier::~HTMLTidier()
{
    tidyRelease( tidyDoc );
}

int HTMLTidier::setErrorBuffer( Buffer& buffer )
{
    return tidySetErrorBuffer( tidyDoc, &buffer );
}

int HTMLTidier::setIntegerOption(  TidyOptionId const optionID, int const newValue )
{
    return tidyOptSetInt( tidyDoc, optionID, newValue );
}

bool HTMLTidier::setBooleanOption( TidyOptionId const optionID, Bool const newValue )
{
    return tidyOptSetBool( tidyDoc, optionID, newValue );
}

int HTMLTidier::ingestMarkdown( std::string const& string )
{
    return tidyParseString( tidyDoc, string.c_str() );
}

int HTMLTidier::cleanAndRepair()
{
    return tidyCleanAndRepair( tidyDoc );
}

int HTMLTidier::saveToBuffer( Buffer& buffer )
{
    return tidySaveBuffer( tidyDoc, &buffer );
}

std::string HTMLTidier::tidyupHTML( std::string const& html )
{
    std::string tidiedHtml;

    setErrorBuffer( errbuf );                       // Capture diagnostics rather than displaying them

    setBooleanOption( TidyMark, no );               // Don't add LibTidy-specific comments to HTML
    setBooleanOption( TidyHtmlOut, yes );           // Convert to HTML
    setBooleanOption( TidyForceOutput, yes );       // Even if there were errors
    setIntegerOption( TidyIndentAttributes, yes );
    setIntegerOption( TidyIndentContent, yes );
    setIntegerOption( TidyIndentCdata, yes );
    setIntegerOption( TidyIndentSpaces, 2 );
    setIntegerOption( TidyWrapLen, 132 );

    ingestMarkdown( html );                            // Parse the input into the document
    cleanAndRepair();                               // Fix errors and re-format the document
    saveToBuffer( output );                         // Pretty-print to the output buffer

    auto const outstr = reinterpret_cast< char const* >( output.bp );
    if ( outstr != nullptr ) tidiedHtml = outstr;
    return tidiedHtml;
}

std::string HTMLTidier::getDiagnostics() const
{
    std::string diagnostics;
    auto const errstr = reinterpret_cast< char const* >( errbuf.bp );
    if ( errstr != nullptr ) diagnostics = errstr;
    return diagnostics;
}
