project( mdview-library CXX )

pkg_check_modules(		LIB_MODULES REQUIRED
						tidy
						tinfo
						gtkmm-4.0
						md4c-html
						magic_enum
						webkitgtk-6.0
						)

#-------

# The executable that will generate X-macros of known GDK keynames
set(	KEYNAMES_CMD	${CMAKE_SOURCE_DIR}/extract-keynames )

# The authoritative source of defined GDK keynames
set(	KEYNAMES_IN		/usr/include/gtk-4.0/gdk/gdkkeysyms.h )

# The X-macros header-file which the library will be including...
set(	KEYNAMES_OUT	${PROJECT_SOURCE_DIR}/gdk-keynames.x-macros )

# This target can be used as a dependency by, e.g., the library build
add_custom_target(		generate-keynames
						DEPENDS ${KEYNAMES_OUT}
						)

# This command defines how the keynames-file will be generated
add_custom_command(		OUTPUT ${KEYNAMES_OUT}
						DEPENDS ${KEYNAMES_IN} ${KEYNAMES_CMD}

    					COMMAND ${KEYNAMES_CMD}
    					ARGS ${KEYNAMES_IN} > ${KEYNAMES_OUT}
						)

#-------

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

# Ensure the library-build will generate the keynames-file, when required
add_dependencies(		mdview-lib generate-keynames )

target_compile_options( mdview-lib PUBLIC ${LIB_MODULES_CFLAGS} )
target_link_libraries( 	mdview-lib PUBLIC ${LIB_MODULES_LDFLAGS} )

set_target_properties( 	mdview-lib PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-lib RUNTIME DESTINATION lib )
