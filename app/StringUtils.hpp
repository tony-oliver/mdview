#ifndef INCLUDED_AWO_STRING_UTILS_HPP
#define INCLUDED_AWO_STRING_UTILS_HPP

#include <string>
#include <vector>
#include <utility>

namespace awo {

using StringMappings = std::vector< std::pair< int, std::string > >;

extern std::string bitmaskToString( int bitmask, StringMappings const& mappings );

} // close namespace awo

#endif // INCLUDED_AWO_STRING_UTILS_HPP
