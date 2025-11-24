project( mdview-application CXX )

pkg_check_modules(		APP_MODULES REQUIRED
						gtkmm-4.0
						magic_enum
						)

add_executable(			mdview-app
                		mdview.cpp
                		Options.cpp
                		MainWindow.cpp
                		SignalHandler.cpp
                		)

set_target_properties( 	mdview-app PROPERTIES OUTPUT_NAME mdview )
target_compile_options( mdview-app PUBLIC ${APP_MODULES_CFLAGS} )
target_link_libraries( 	mdview-app PUBLIC ${APP_MODULES_LDFLAGS} )

target_link_libraries( 	mdview-app PUBLIC mdview-lib )

install( TARGETS mdview-app RUNTIME DESTINATION bin )
