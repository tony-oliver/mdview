#include "MainWindow.hpp"

namespace { // unnamed
std::string path = "/usr/local/lib64/";
} // close unnamed namespace

MainWindow::MainWindow( Options const& options )
: logger{ options.get_logger() }
, filename{ options.get_filename() }
, make_thread_safe( logger, options.get_use_colour() )
, markdown_view( *this, logger, filename, options.get_dump_html(), options.get_show_diagnostics() )
{
    // Arrange to close this main window when any critical signal is encountered.
    signal_handler.registerAction( [ & ]{ hide(); } );

    // Determine appropriate title of this window.
    auto const prgname = g_get_prgname();

    if ( ( prgname != nullptr ) && !filename.empty() )
    {
        set_title( prgname + ( " - " + filename ) );
     }

    // Set window parameters, just prior to the application displaying it.
    set_child( markdown_view );
    set_default_size( 1200, 800 );
}

//============================================================================
