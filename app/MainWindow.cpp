#include "MainWindow.hpp"

#include <gtkmm/version.h>              // GTKMM_*_VERSION
#include <webkit/webkit.h>              // WEBKIT_*_VERSION, webkit_get_*_version()
#include "tidy.h"                       // tidyLibraryVersion()
#include <magic_enum/magic_enum.hpp>    // MAGIC_ENUM_VERSION_*

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

constexpr std::string appname = "mdview";

//----------------------------------------------------------------------------

void log_library_versions_to( std::ostream& logger )
{
    static std::string const delimiter = "--------------------------------";

    logger << delimiter << std::endl;

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

    logger << "MagicEnum header version = "
           << MAGIC_ENUM_VERSION_MAJOR << "."
           << MAGIC_ENUM_VERSION_MINOR << "."
           << MAGIC_ENUM_VERSION_PATCH << std::endl;

    logger << delimiter << std::endl;
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Options const& options )
: logger{ options.get_logger() }
, filename{ options.get_filename() }
, makeThreadSafe( logger, options.get_use_colour() )
, markdownView( logger, filename, options.get_dump_html(), options.get_show_diagnostics() )
{
    log_library_versions_to( logger );

    markdownView.render();

    // Arrange to close this main window when any critical signal is encountered.
    signalHandler.registerAction( [ & ]{ hide(); } );

    auto title = appname;
    if ( !filename.empty() ) title += " - " + filename;

    set_title( title );
    set_child( markdownView );
    set_default_size( 1200, 800 );
}

//============================================================================
