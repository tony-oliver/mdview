#include "Options.hpp"

#include <iostream>
#include <streambuf>

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

struct NullBuffer: std::streambuf
{
};

NullBuffer nullbuffer;

//----------------------------------------------------------------------------

struct NullStream: std::ostream
{
    NullStream()
    : std::ostream{ &nullbuffer }
    {
    }
};

NullStream nullstream;

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

Options::Options( int const argc, char** const argv )
: logger_ptr{ &nullstream }
{
    constexpr char arguments_doc[]   // description of non-option arguments
    {
        "FILE"
    };

    constexpr char program_doc[]     // description of program and its non-option arguments
    {
        "\n"
        "mdview -- a markdown-file viewer.\n"
        "\n"
        "OPTIONS:\n"
        "\v"
    };

    constexpr argp_option options[]
    {
        { "verbose",        'v', nullptr, 0, "Produce verbose output on stderr", 0 },
        { "html",           'h', nullptr, 0, "Dump HTML to stdout", 0 },
        { "colour",         'x', nullptr, 0, "Distinguish verbose output by colours", 0 },
        { "foreground",     'f', nullptr, 0, "Do not detach and run 'in the background'", 0 },
        { "diagnostics",    'd', nullptr, 0, "Show diagnostics from LibTidy", 0 },
        { "cmark",          'c', nullptr, 0, "Use cmark library for conversion", 0 },
        { "sundown",        's', nullptr, 0, "Use sundown library for conversion", 0 },
        { "cmark",          'm', nullptr, 0, "Use md4c library for conversion (default)", 0 },
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
    case 'h': dump_html         = true;     break;
    case 'x': use_colour        = true;     break;
    case 'f': foreground        = true;     break;
    case 'd': show_diagnostics  = true;     break;
    case 'c': converter         = CMark;    break;
    case 's': converter         = Sundown;  break;
    case 'm': converter         = MD4C;     break;
    case 'v': logger_ptr = &std::clog;      break;

    case ARGP_KEY_ARG:
        if ( arg_num > 0 ) argp_usage( state ); // Too many arguments.
        filename = arg;
        break;

    case ARGP_KEY_END:
        if ( arg_num < 1 ) argp_usage( state ); // Insufficient arguments.
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

bool Options::get_foreground() const
{
    return foreground;
}

bool Options::get_show_diagnostics() const
{
    return show_diagnostics;
}

Converter Options::get_converter() const
{
    return converter;
}

std::ostream& Options::get_logger() const
{
    return *logger_ptr;
}
