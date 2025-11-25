#include "MainWindow.hpp"

#include <magic_enum/magic_enum.hpp>    // MAGIC_ENUM_VERSION_*
#include <ncurses/ncurses.h>            // NCURSES_VERSION_*, ncurses_version()
#include <webkit/webkit.h>              // WEBKIT_*_VERSION, webkit_get_*_version()
#include <tidy.h>                       // tidyLibraryVersion()

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

void log_library_versions_to( std::ostream& logger )
{
    static std::string const delimiter
    {
        "----------------------------------------------"
    };

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

    logger << "NCurses header version = "
           << NCURSES_VERSION_MAJOR << "."
           << NCURSES_VERSION_MINOR << "."
           << NCURSES_VERSION_PATCH << std::endl;

    logger << "NCurses library version = "
           << curses_version() << std::endl;

    logger << delimiter << std::endl;
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

MainWindow::MainWindow( Options const& options )
: logger{ options.get_logger() }
, filename{ options.get_filename() }
, makeThreadSafe( logger, options.get_use_colour() )
, markdownView( *this, logger, filename, options.get_dump_html(), options.get_show_diagnostics() )
{
    // Print out versions of third-party components used.
    log_library_versions_to( logger );

    // Arrange to close this main window when any critical signal is encountered.
    signalHandler.registerAction( [ & ]{ hide(); } );

    // Determine appropriate title of this window.
    auto const prgname = g_get_prgname();

    if ( ( prgname != nullptr ) && !filename.empty() )
    {
        set_title( prgname + ( " - " + filename ) );
     }

    // Set window parameters, just prior to the application displaying it.
    set_child( markdownView );
    set_default_size( 1200, 800 );
}

//============================================================================
