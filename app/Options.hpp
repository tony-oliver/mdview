#ifndef INCLUDED_MDVIEW_OPTIONS_HPP
#define INCLUDED_MDVIEW_OPTIONS_HPP

#include <argp.h>

#include <string>
#include <ostream>

class Options
{
    std::string filename;
    bool dump_html = false;
    bool use_colour = false;
    std::ostream* logger_ptr;

public:

    Options( int argc, char** argv );

    std::string const& get_filename() const;
    bool get_dump_html() const;
    bool get_use_colour() const;
    std::ostream& get_logger() const;

private:

    static error_t option_parser( int key, char* arg, argp_state* state );
    error_t parse_option( int key, char* arg, argp_state* state );
};

#endif // INCLUDED_MDVIEW_OPTIONS_HPP
