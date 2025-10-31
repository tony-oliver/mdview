#include "MainWindow.hpp"

#include <gtkmm/version.h>
#include <webkit/webkit.h>  // WEBKIT_*_VERSION, webkit_get_*_version()
#include "tidy.h"           // tidyLibraryVersion()

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

//----------------------------------------------------------------------------

void log_components_to( std::ostream& logger )
{
    logger << "-------------------------------" << std::endl;

    logger << "GTKmm header version = "
           << GTKMM_MAJOR_VERSION << "."
           << GTKMM_MINOR_VERSION << "."
           << GTKMM_MICRO_VERSION << std::endl;

    // Desired: GTKmm library version

    logger << "WebKit header version = "
           << WEBKIT_MAJOR_VERSION << "."
           << WEBKIT_MINOR_VERSION << "."
           << WEBKIT_MICRO_VERSION << std::endl;

    logger << "WebKit library version = "
            << webkit_get_major_version() << "."
            << webkit_get_minor_version() << "."
            << webkit_get_micro_version() << std::endl;

    // Desired: MD4C header version
    // Desired: MD4C library version
    // Desired: LibTidy header version

    logger << "LibTidy library version = "
           << tidyLibraryVersion() << std::endl;

    logger << "-------------------------------" << std::endl;
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Options const& options )
: options{ options }
, filename{ options.get_filename() }
, logger{ options.get_logger() }
, makeThreadSafe( logger, options.get_use_colour() )
, markdownView( filename, logger, options.get_dump_html(), options.get_show_diagnostics() )
{
    signalHandler.registerAction( [ & ]
    {
        logger << "Calling Gtk::Widget::hide() on main window" << std::endl;
        hide(); // less error-prone than close()
    } );

    auto title = appname;
    if ( !filename.empty() ) title += " - " + filename;

    set_title( title );
    set_child( markdownView );
    set_default_size( 1200, 800 );

    log_components_to( logger );

    markdownView.render();
}

//============================================================================
