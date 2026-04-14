#include "NullStream.hpp"

#include <streambuf>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

// This is just an alias for the std::streambuf class,
// but with a publicly-accessible default constructor.

struct NullBuffer: std::streambuf
{
};

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

std::ostream& get_nullstream()
{
    static NullBuffer nullbuffer;

    static std::ostream nullstream( &nullbuffer );

    return nullstream;
}
