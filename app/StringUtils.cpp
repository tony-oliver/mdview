#include "StringUtils.hpp"

#include <iomanip>
#include <ostream>
#include <sstream>

namespace awo {

std::string bitmaskToString( int value, StringMappings const& mappings )
{
    std::ostringstream oss;
    std::string delimiter;

    for ( auto const& [ mappedBitmask, name ]: mappings )
    {
        if ( ( value & mappedBitmask ) == mappedBitmask )
        {
            value ^= mappedBitmask;
            oss << delimiter << name;
            if ( delimiter.empty() ) delimiter = " | ";
        }
    }

    if ( value != 0 )
    {
        oss << std::hex << std::uppercase << std::setfill( '0' );
        oss << delimiter << "0x" << std::setw( ( sizeof value ) / 4 ) << value;
    }

    return oss.str();
}

} // close namespace awo
