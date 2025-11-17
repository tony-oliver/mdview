#ifndef INCLUDED_NULLSTREAM_HPP
#define INCLUDED_NULLSTREAM_HPP

#include <ostream>

// Returns a reference to a singleton instance of
// an ostream object that writes its output nowhere.
// Equivalent to std::ofstream( "/dev/null" ) in
// a POSIX runtime environment.

std::ostream& get_nullstream();

#endif // INCLUDED_NULLSTREAM_HPP
