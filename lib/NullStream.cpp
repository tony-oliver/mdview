#include "NullStream.hpp"

#include <streambuf>

std::ostream& get_nullstream()
{
    static struct NullBuffer: std::streambuf {} nullbuffer;
    static std::ostream nullstream( &nullbuffer );

    return nullstream;
}
