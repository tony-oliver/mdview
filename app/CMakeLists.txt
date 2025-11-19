project( mdview-application CXX )

add_executable(	mdview-app
                mdview.cpp
                Options.cpp
                MainWindow.cpp
                SignalHandler.cpp
                )

target_link_libraries( mdview-app PUBLIC mdview-lib )

set_target_properties( mdview-app PROPERTIES OUTPUT_NAME mdview )

install( TARGETS mdview-app RUNTIME DESTINATION bin )
