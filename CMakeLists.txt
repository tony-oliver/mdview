cmake_minimum_required( VERSION 3.27 FATAL_ERROR )

project( mdview )

set(	CMAKE_CXX_STANDARD			20	)
set(	CMAKE_CXX_STANDARD_REQUIRED On	)
set(	CMAKE_CXX_EXTENSIONS 		Off )

add_compile_options( -pedantic )
add_compile_options( -Wall -Wextra )
add_compile_options( -O2 )
add_compile_options( -g3 )

add_subdirectory( app )
