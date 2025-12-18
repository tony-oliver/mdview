project( mdview-library CXX )

pkg_check_modules(		LIB_MODULES REQUIRED
						tidy
						tinfo
						gtkmm-4.0
						md4c-html
						magic_enum
						webkitgtk-6.0
						)

set(	KEYNAMES_CMD	${CMAKE_SOURCE_DIR}/extract-keynames )
set(	KEYNAMES_IN		/usr/include/gtk-4.0/gdk/gdkkeysyms.h )
set(	KEYNAMES_OUT	${PROJECT_SOURCE_DIR}/gdk-keynames.x-macros )

add_custom_command(		OUTPUT ${KEYNAMES_OUT}
						DEPENDS ${KEYNAMES_IN} ${KEYNAMES_CMD}

    					COMMAND ${KEYNAMES_CMD}
    					ARGS ${KEYNAMES_IN} > ${KEYNAMES_OUT}
						)

add_custom_target(		generate-keynames
						DEPENDS ${KEYNAMES_OUT}
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

add_dependencies(		mdview-lib generate-keynames )

target_compile_options( mdview-lib PUBLIC ${LIB_MODULES_CFLAGS} )
target_link_libraries( 	mdview-lib PUBLIC ${LIB_MODULES_LDFLAGS} )

set_target_properties( 	mdview-lib PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-lib RUNTIME DESTINATION lib )
