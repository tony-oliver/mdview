cmake_minimum_required( VERSION 3.11 FATAL_ERROR )

project( mdview CXX )

include( FindPkgConfig )
include( GNUInstallDirs )
include( CMakePrintHelpers )

cmake_print_variables( CMAKE_INSTALL_LIBDIR )
cmake_print_variables( CMAKE_INSTALL_PREFIX )
cmake_print_variables( CMAKE_INSTALL_FULL_LIBDIR )

set( CMAKE_CXX_STANDARD_REQUIRED On )
set( CMAKE_CXX_STANDARD          23 )
set( CMAKE_CXX_EXTENSIONS       Off )

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
