#include "MainWindow.hpp"

#include <gtkmm/version.h>
#include <webkit/webkit.h>  // WEBKIT_*_VERSION, webkit_get_*_version()
#include "tidy.h"           // tidyLibraryVersion()

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

//----------------------------------------------------------------------------

void log_library_versions_to( std::ostream& logger )
{
    logger << "-------------------------------" << std::endl;

    logger << "GTKmm header version = "
           << GTKMM_MAJOR_VERSION << "."
           << GTKMM_MINOR_VERSION << "."
           << GTKMM_MICRO_VERSION << std::endl;

    logger << "WebKit header version = "
           << WEBKIT_MAJOR_VERSION << "."
           << WEBKIT_MINOR_VERSION << "."
           << WEBKIT_MICRO_VERSION << std::endl;

    logger << "WebKit library version = "
            << webkit_get_major_version() << "."
            << webkit_get_minor_version() << "."
            << webkit_get_micro_version() << std::endl;

    logger << "LibTidy library version = "
           << tidyLibraryVersion() << std::endl;

    logger << "-------------------------------" << std::endl;
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Options const& options )
: logger{ options.get_logger() }
, makeThreadSafe( logger, options.get_use_colour() )
, markdownView( logger, options.get_dump_html(), options.get_show_diagnostics() )
{
    log_library_versions_to( logger );

    auto const filename{ options.get_filename() };
    markdownView.render( filename );

    // Arrange to close this main window when any critical signal is encountered.
    signalHandler.registerAction( [ & ]{ hide(); } );

    auto title = appname;
    if ( !filename.empty() ) title += " - " + filename;

    set_title( title );
    set_child( markdownView );
    set_default_size( 1200, 800 );
}

//============================================================================
