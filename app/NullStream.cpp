#include "NullStream.hpp"

#include <streambuf>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

struct NullBuffer: std::streambuf
{
};

NullBuffer nullbuffer;

//----------------------------------------------------------------------------

struct NullStream: std::ostream
{
    NullStream()
    : std::ostream{ &nullbuffer }
    {
    }
};

NullStream nullstream;

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

std::ostream& get_nullstream()
{
    return nullstream;
}
