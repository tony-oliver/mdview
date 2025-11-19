project( mdview-documentation )

# ensure Doxygen is installed

find_package( Doxygen REQUIRED )

# Set variable(s) defining the documentation source files

set( SOURCE_FILES ${CMAKE_SOURCE_DIR}/lib )

# Copy the configuration file from source to build area, expanding variables

configure_file( Doxyfile . )

# Execute Doxygen with the configured Doxyfile that's now in the build area

add_custom_target(  mdview-docs			ALL
                    WORKING_DIRECTORY	${CMAKE_CURRENT_BINARY_DIR}
                    COMMAND 			${DOXYGEN_EXECUTABLE} -q
                    )
