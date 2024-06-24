#include "Options.hpp"

#include <iostream>

namespace {
std::ostream nullstream{ nullptr };
}

Options::Options( int const argc, char** const argv )
: logger_ptr{ &nullstream }
{
    static char const arguments_doc[] = "FILE";  // description of non-option arguments

    static char const program_doc[] =   // description of program and its non-option arguments
        "\n"
        "mdview -- a markdown-file viewer.\n"
        "\n"
        "OPTIONS:\n"
        "\v";

    static argp_option const options[] =
    {
        { "dump-html",  'd', nullptr, 0, "Dump HTML to stdout", 0 },
        { "colour",     'c', nullptr, 0, "Distinguish verbose output by colours", 0 },
        { "verbose",    'v', nullptr, 0, "Produce verbose output on stderr", 0 },
        {}
    };

    static argp const parser
    {
        options,
        option_parser,
        arguments_doc,
        program_doc,
        nullptr,    // children
        nullptr,    // help_filter
        nullptr     // i18n domain
    };

    argp_parse( &parser, argc, argv, 0, nullptr, this );
}

error_t Options::option_parser( int const key, char* const arg, argp_state* const state )
{
    return static_cast< Options* >( state->input )->parse_option( key, arg, state );
}

error_t Options::parse_option( int const key, char* const arg, argp_state* const state )
{
    auto const arg_num = state->arg_num;

    switch ( key )
    {
    case 'd': dump_html  = true;        break;
    case 'c': use_colour = true;        break;
    case 'v': logger_ptr = &std::clog;  break;

    case ARGP_KEY_ARG:
        if ( arg_num > 0 ) argp_usage( state ); // Too many arguments.
        filename = arg;
        break;

    case ARGP_KEY_END:
        if ( arg_num == 0 ) argp_usage( state ); // Insufficient arguments.
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

std::string const& Options::get_filename() const
{
    return filename;
}

bool Options::get_dump_html() const
{
    return dump_html;
}

bool Options::get_use_colour() const
{
    return use_colour;
}

std::ostream& Options::get_logger() const
{
    return *logger_ptr;
}
