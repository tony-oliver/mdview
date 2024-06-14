project( mdview )

add_executable( mdview
				mdview.cpp
				MainWindow.cpp
				WebView.cpp
				ThreadLoop.cpp
				FileWatcher.cpp
				SignalHandler.cpp
				)

pkg_search_module( GTKMM REQUIRED gtkmm-4.0 )
target_compile_options( mdview PUBLIC ${GTKMM_CFLAGS} )
target_link_libraries( mdview PUBLIC ${GTKMM_LDFLAGS} )

pkg_search_module( WEBKIT REQUIRED webkitgtk-6.0 )
target_compile_options( mdview PUBLIC ${WEBKIT_CFLAGS} )
target_link_libraries( mdview PUBLIC ${WEBKIT_LDFLAGS} )

pkg_search_module( LIBCMARK REQUIRED libcmark )
target_compile_options( mdview PUBLIC ${LIBCMARK_CFLAGS} )
target_link_libraries( mdview PUBLIC ${LIBCMARK_LDFLAGS} )

install( TARGETS mdview RUNTIME DESTINATION bin )
