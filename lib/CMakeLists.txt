project( mdview-library LANGUAGES CXX )

pkg_check_modules(		LIB_MODULES REQUIRED
						tidy
						tinfo
						gtkmm-4.0
						md4c-html
						magic_enum
						webkitgtk-6.0
						)

add_library( 			mdview-lib SHARED
						argp_version.cpp
			 			WebView.cpp
			 			KeyMatch.cpp
			 			HTMLTidier.cpp
			 			NullStream.cpp
			 			ThreadLoop.cpp
			 			FileWatcher.cpp
			 			MDConverter.cpp
			 			StringUtils.cpp
			 			MarkdownView.cpp
			 			SearchDialog.cpp
			 			FindController.cpp
			 			SuperCheckButton.cpp
			 			ThreadSafeOStream.cpp
             			)

target_compile_options( mdview-lib PUBLIC ${LIB_MODULES_CFLAGS} )
target_link_libraries( 	mdview-lib PUBLIC ${LIB_MODULES_LDFLAGS} )

set_target_properties( 	mdview-lib PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-lib RUNTIME DESTINATION lib )
