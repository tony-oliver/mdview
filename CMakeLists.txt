cmake_minimum_required( VERSION 4.0 FATAL_ERROR )

project( mdview LANGUAGES CXX )

include( FindPkgConfig )

set(	CMAKE_CXX_STANDARD_REQUIRED On	)
set(	CMAKE_CXX_STANDARD			23	)
set(	CMAKE_CXX_EXTENSIONS 		Off )

add_compile_options( -pedantic )
add_compile_options( -Wall -Wextra )
add_compile_options( -O2 )
add_compile_options( -g3 )
add_compile_options( -fPIC )
add_compile_options( -march=native )

include_directories( lib )

add_subdirectory( app )
add_subdirectory( lib )
add_subdirectory( docs )
