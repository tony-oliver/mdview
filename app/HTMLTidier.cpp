#include "HTMLTidier.hpp"

//============================================================================

HTMLTidier::Buffer::Buffer()
: TidyBuffer()
{
    tidyBufInit( this );
}

//----------------------------------------------------------------------------

HTMLTidier::Buffer::~Buffer()
{
    tidyBufFree( this );
}

//----------------------------------------------------------------------------

void HTMLTidier::Buffer::clear()
{
    tidyBufClear( this );
}

//----------------------------------------------------------------------------

HTMLTidier::Buffer::operator std::string() const
{
    std::string contents;
    auto const text_string = reinterpret_cast< char const* >( bp );
    if ( text_string != nullptr ) contents.assign( text_string );
    return contents;
}

//============================================================================

HTMLTidier::HTMLTidier()
: html_tidier{ tidyCreate() }
{
    setBooleanOption( TidyMark, no );               // Don't add LibTidy-specific comments to output
    setBooleanOption( TidyHtmlOut, yes );           // Convert to HTML ...
    setBooleanOption( TidyForceOutput, yes );       // ... even if there were errors
    setIntegerOption( TidyIndentAttributes, yes );
    setIntegerOption( TidyIndentContent, yes );
    setIntegerOption( TidyIndentCdata, yes );
    setIntegerOption( TidyIndentSpaces, 2 );
    setIntegerOption( TidyWrapLen, 132 );
}

//----------------------------------------------------------------------------

HTMLTidier::~HTMLTidier()
{
    tidyRelease( html_tidier );
}

//============================================================================


bool HTMLTidier::setBooleanOption( TidyOptionId const option_id, Bool const new_value )
{
    return tidyOptSetBool( html_tidier, option_id, new_value );
}

//----------------------------------------------------------------------------

int HTMLTidier::setIntegerOption(  TidyOptionId const option_id, int const new_value )
{
    return tidyOptSetInt( html_tidier, option_id, new_value );
}
//============================================================================

int HTMLTidier::saveErrorsToBuffer( Buffer& buffer )
{
    return tidySetErrorBuffer( html_tidier, &buffer );
}

//----------------------------------------------------------------------------

int HTMLTidier::saveOutputToBuffer( Buffer& buffer )
{
    return tidySaveBuffer( html_tidier, &buffer );
}

//============================================================================

int HTMLTidier::ingestContent( std::string const& content )
{
    return tidyParseString( html_tidier, content.c_str() );
}

//----------------------------------------------------------------------------

int HTMLTidier::cleanAndRepair()
{
    return tidyCleanAndRepair( html_tidier );
}

//============================================================================

std::string HTMLTidier::tidyupHTML( std::string const& html )
{
    saveErrorsToBuffer( diagnostics_buffer );       // Capture diagnostics rather than displaying them
    ingestContent( html );                          // Parse the input into the document
    cleanAndRepair();                               // Fix errors and re-format the document
    saveOutputToBuffer( output_buffer );            // Pretty-print to the output buffer

    return output_buffer;
}

//----------------------------------------------------------------------------

std::string HTMLTidier::getDiagnostics() const
{
    return diagnostics_buffer;
}

//============================================================================
