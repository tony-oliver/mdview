cmake_minimum_required( VERSION 3.27 FATAL_ERROR )
project( mdview )

include( FindPkgConfig )

set( CMAKE_CXX_STANDARD 20 )							# uses std::map<>::contains()
set( CMAKE_CXX_EXTENSIONS Off )
set( CMAKE_CXX_STANDARD_REQUIRED On )
add_compile_options( -pedantic -Wall -Wextra -O3 -g3 )

add_subdirectory( app )
