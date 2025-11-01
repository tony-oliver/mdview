#ifndef INCLUDED_MDVIEW_OPTIONS_HPP
#define INCLUDED_MDVIEW_OPTIONS_HPP

#include <argp.h>

#include <string>
#include <ostream>

class Options
{
    std::string     filename;
    bool            dump_html           = false;
    bool            use_colour          = false;
    bool            show_diagnostics    = false;
    std::ostream*   logger_ptr          = nullptr;

public:

    Options( int argc, char** argv );

    std::string const&  get_filename()          const { return filename; }
    bool                get_dump_html()         const { return dump_html; }
    bool                get_use_colour()        const { return use_colour; }
    bool                get_show_diagnostics()  const { return show_diagnostics; }
    std::ostream&       get_logger()            const { return *logger_ptr; }

private:

    static error_t parser( int key, char* arg, argp_state* state );
    error_t        option( int key, char* arg, argp_state* state );
};

#endif // INCLUDED_MDVIEW_OPTIONS_HPP
