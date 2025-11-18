#include "HTMLTidier.hpp"

HTMLTidier::Buffer::Buffer()
{
    tidyBufInit( this );
}

HTMLTidier::Buffer::~Buffer()
{
    tidyBufFree( this );
}

HTMLTidier::HTMLTidier()
: tidy_doc{ tidyCreate() }
{
}

HTMLTidier::~HTMLTidier()
{
    tidyRelease( tidy_doc );
}

int HTMLTidier::setErrorBuffer( Buffer& buffer )
{
    return tidySetErrorBuffer( tidy_doc, &buffer );
}

int HTMLTidier::setIntegerOption(  TidyOptionId const option_id, int const new_value )
{
    return tidyOptSetInt( tidy_doc, option_id, new_value );
}

bool HTMLTidier::setBooleanOption( TidyOptionId const option_id, Bool const new_value )
{
    return tidyOptSetBool( tidy_doc, option_id, new_value );
}

int HTMLTidier::ingestContent( std::string const& content )
{
    return tidyParseString( tidy_doc, content.c_str() );
}

int HTMLTidier::cleanAndRepair()
{
    return tidyCleanAndRepair( tidy_doc );
}

int HTMLTidier::saveToBuffer( Buffer& buffer )
{
    return tidySaveBuffer( tidy_doc, &buffer );
}

std::string HTMLTidier::tidyupHTML( std::string const& html )
{
    setBooleanOption( TidyMark, no );               // Don't add LibTidy-specific comments to output
    setBooleanOption( TidyHtmlOut, yes );           // Convert to HTML ...
    setBooleanOption( TidyForceOutput, yes );       // ... even if there were errors
    setIntegerOption( TidyIndentAttributes, yes );
    setIntegerOption( TidyIndentContent, yes );
    setIntegerOption( TidyIndentCdata, yes );
    setIntegerOption( TidyIndentSpaces, 2 );
    setIntegerOption( TidyWrapLen, 132 );

    setErrorBuffer( diagnostics_buffer );           // Capture diagnostics rather than displaying them
    ingestContent( html );                          // Parse the input into the document
    cleanAndRepair();                               // Fix errors and re-format the document
    saveToBuffer( output_buffer );                  // Pretty-print to the output buffer
    return contentsOfBuffer( output_buffer );
}

std::string HTMLTidier::getDiagnostics() const
{
    return contentsOfBuffer( diagnostics_buffer );
}

std::string HTMLTidier::contentsOfBuffer( Buffer const& buffer ) const
{
    std::string contents;
    auto const data_pointer = reinterpret_cast< char const* >( buffer.bp );
    if ( data_pointer != nullptr ) contents = data_pointer;
    return contents;
}
