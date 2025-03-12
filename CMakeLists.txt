cmake_minimum_required( VERSION 3.27 FATAL_ERROR )

project( mdview )

add_compile_options( -O2 -g3 )

include_directories(	lib/sundown/src
						lib/sundown/html
						)
						
add_subdirectory( lib )
add_subdirectory( app )
