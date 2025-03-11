#include "../md2html/TextBuffer.hpp"

//============================================================================

TextBuffer::TextBuffer( std::size_t const unit_size )
: buffer{ bufnew( unit_size ) }
{
}

//----------------------------------------------------------------------------

TextBuffer::~TextBuffer()
{
    bufrelease( buffer );
}

//----------------------------------------------------------------------------

TextBuffer::operator buf*()
{
    return buffer;
}

//----------------------------------------------------------------------------

TextBuffer::operator std::string() const
{
    return std::string( reinterpret_cast< char const* >( buffer->data ), buffer->size );
}

//============================================================================
