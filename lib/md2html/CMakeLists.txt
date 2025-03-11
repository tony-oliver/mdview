PROJECT( mdview-libmd2html )

set( CMAKE_CXX_STANDARD 			20 	)
set( CMAKE_CXX_STANDARD_REQUIRED 	On 	)
set( CMAKE_CXX_EXTENSIONS 			Off )

add_compile_options( -pedantic      )
add_compile_options( -Wall -Wextra  )
add_compile_options( -O2 -g3        )

add_library( 			md2html
                		HTMLRenderer.cpp
                		MarkdownParser.cpp
                		TextBuffer.cpp
		                )

include_directories( 	../sundown/src
                        ../sundown/html
                        )

target_link_libraries(	md2html
                        sundown
                        )
