project( mdview-application LANGUAGES CXX )

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

target_compile_options( mdview-app PUBLIC ${APP_MODULES_CFLAGS} )
target_link_libraries( 	mdview-app PUBLIC ${APP_MODULES_LDFLAGS} )
target_link_libraries( 	mdview-app PUBLIC mdview-lib )

set_target_properties( 	mdview-app PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-app RUNTIME DESTINATION bin )
