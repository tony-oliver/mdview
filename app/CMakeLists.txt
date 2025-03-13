project( mdview-app )

include( FindPkgConfig )

set(	CMAKE_CXX_STANDARD			20	)
set(	CMAKE_CXX_STANDARD_REQUIRED On	)
set(	CMAKE_CXX_EXTENSIONS 		Off )

add_compile_options( -pedantic -Wall -Wextra )

add_executable( 		mdview
                		mdview.cpp
                		Options.cpp
                		WebView.cpp
                		HTMLTidier.cpp
                		MainWindow.cpp
                		ThreadLoop.cpp
                		FileWatcher.cpp
                		StringUtils.cpp
                		SignalHandler.cpp
                		MarkdownConverter.cpp
                		ThreadSafeOStream.cpp
                		)

pkg_search_module( TIDY REQUIRED tidy )
target_compile_options( mdview PUBLIC ${TIDY_CFLAGS} )
target_link_libraries( mdview PUBLIC ${TIDY_LDFLAGS} )

pkg_search_module( GTKMM REQUIRED gtkmm-4.0 )
target_compile_options( mdview PUBLIC ${GTKMM_CFLAGS} )
target_link_libraries( mdview PUBLIC ${GTKMM_LDFLAGS} )

pkg_search_module( WEBKIT REQUIRED webkitgtk-6.0 )
target_compile_options( mdview PUBLIC ${WEBKIT_CFLAGS} )
target_link_libraries( mdview PUBLIC ${WEBKIT_LDFLAGS} )

pkg_search_module( LIBCMARK REQUIRED libcmark )
target_compile_options( mdview PUBLIC ${LIBCMARK_CFLAGS} )
target_link_libraries( mdview PUBLIC ${LIBCMARK_LDFLAGS} )

pkg_search_module( TERMINFO REQUIRED tinfo )
target_compile_options( mdview PUBLIC ${TERMINFO_CFLAGS} )
target_link_libraries( mdview PUBLIC ${TERMINFO_LDFLAGS} )

pkg_search_module( MD4C_HTML REQUIRED md4c-html )
target_compile_options( mdview PUBLIC ${MD4C_HTML_CFLAGS} )
target_link_libraries( mdview PUBLIC ${MD4C_HTML_LDFLAGS} )

include_directories( ../lib/md2html )

target_link_libraries(	mdview PUBLIC
						md2html
						sundown
						)

install( TARGETS mdview RUNTIME DESTINATION bin )
