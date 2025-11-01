project( mdview-app )

include( FindPkgConfig )

add_executable(	mdview
                mdview.cpp
                Options.cpp
                WebView.cpp
                HTMLTidier.cpp
                MainWindow.cpp
                NullStream.cpp
                ThreadLoop.cpp
                FileWatcher.cpp
                MDConverter.cpp
                StringUtils.cpp
                MarkdownView.cpp
                SignalHandler.cpp
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

pkg_search_module( TERMINFO REQUIRED tinfo )
target_compile_options( mdview PUBLIC ${TERMINFO_CFLAGS} )
target_link_libraries( mdview PUBLIC ${TERMINFO_LDFLAGS} )

pkg_search_module( MD4C_HTML REQUIRED md4c-html )
target_compile_options( mdview PUBLIC ${MD4C_HTML_CFLAGS} )
target_link_libraries( mdview PUBLIC ${MD4C_HTML_LDFLAGS} )

install( TARGETS mdview RUNTIME DESTINATION bin )
