#ifndef INCLUDED_TEXT_BUFFER_HPP
#define INCLUDED_TEXT_BUFFER_HPP

#include "buffer.h" // buf{}

#include <string>   // std::string{}
#include <cstddef>  // std::size_t

//============================================================================

class TextBuffer
{
    buf* buffer;

public:

    TextBuffer( std::size_t unit_size = 64 );

    ~TextBuffer();

    operator buf*();

    operator std::string() const;
};

//============================================================================

#endif // INCLUDED_TEXT_BUFFER_HPP
