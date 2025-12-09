#include "MainWindow.hpp"

//============================================================================

MainWindow::MainWindow( Options const& options )
: logger{ options.get_logger() }
, filename{ options.get_filename() }
, make_thread_safe( logger, options.get_use_colour() )
, markdown_view( *this, logger, filename, options.get_dump_html(), options.get_show_diagnostics() )
{
    // Arrange to close this main window when any critical signal is encountered.
    signal_handler.registerAction( [ & ]{ hide(); } );

    set_child( markdown_view );
    set_default_size( 1200, 800 );
    set_title( g_get_prgname() + ( " - " + filename ) );
}

//============================================================================
