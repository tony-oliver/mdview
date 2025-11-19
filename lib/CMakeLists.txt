project( mdview-library CXX )

add_library( mdview-lib SHARED
			 FileWatcher.cpp
			 HTMLTidier.cpp
			 MarkdownView.cpp
			 MDConverter.cpp
			 NullStream.cpp
			 StringUtils.cpp
			 ThreadLoop.cpp
			 ThreadSafeOStream.cpp
			 WebView.cpp
             )

pkg_search_module( GTKMM REQUIRED gtkmm-4.0 )
target_compile_options( mdview-lib PUBLIC ${GTKMM_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${GTKMM_LDFLAGS} )

pkg_search_module( WEBKIT REQUIRED webkitgtk-6.0 )
target_compile_options( mdview-lib PUBLIC ${WEBKIT_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${WEBKIT_LDFLAGS} )

pkg_search_module( MD4C_HTML REQUIRED md4c-html )
target_compile_options( mdview-lib PUBLIC ${MD4C_HTML_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${MD4C_HTML_LDFLAGS} )

pkg_search_module( TIDY REQUIRED tidy )
target_compile_options( mdview-lib PUBLIC ${TIDY_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${TIDY_LDFLAGS} )

pkg_search_module( TERMINFO REQUIRED tinfo )
target_compile_options( mdview-lib PUBLIC ${TERMINFO_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${TERMINFO_LDFLAGS} )

pkg_search_module( MAGIC_ENUM REQUIRED magic_enum )
target_compile_options( mdview-lib PUBLIC ${MAGIC_ENUM_CFLAGS} )
target_link_libraries( mdview-lib PUBLIC ${MAGIC_ENUM_LDFLAGS} )

set_target_properties( mdview-lib PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-lib RUNTIME DESTINATION lib )
