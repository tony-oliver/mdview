cmake_minimum_required( VERSION 3.27 FATAL_ERROR )
project( markdown )

include( FindPkgConfig )
include( CMakePrintHelpers )

set( CMAKE_CXX_STANDARD_REQUIRED On )
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_EXTENSIONS Off )
add_compile_options( -pedantic -Wall -Wextra -O3 -g3 )

add_subdirectory( src )
