#include "Options.hpp"
#include "NullStream.hpp"

#include <iostream>
#include <streambuf>

char const* argp_program_version      = "version 1.0.1";
char const* argp_program_bug_address  = "tony@oliver.net";

//============================================================================

Options::Options( int const argc, char** const argv )
: logger_ptr{ &get_nullstream() }
{
    enum OptionGroup
    {
        GeneralGroup,

        NumOptionGroups
    };

    constexpr argp_option options[]
    {
        { "html",           'h', nullptr, 0, "Dump HTML to stdout",                     GeneralGroup },
        { "verbose",        'v', nullptr, 0, "Produce verbose output on stderr",        GeneralGroup },
        { "colour",         'x', nullptr, 0, "Distinguish verbose output by colours",   GeneralGroup },
        { "diagnostics",    'd', nullptr, 0, "Show LibTidy diagnostics on stderr",      GeneralGroup },
        {}
    };

    constexpr char args_doc[]   // description of non-option arguments
    {
        "<MDFILE>"
    };

    constexpr char doc[]        // description of program and its non-option arguments
    {
        "\n"
        "mdview -- a markdown-file viewer.\n"
        "\n"
        "OPTIONS:\n"
        //----------------------------------------
        // formatted options will get printed here
        //----------------------------------------
        "\v"
        "ARGUMENTS:\n"
        "\n"
        "  <MDFILE>                   Path to file containing markdown source"
    };

    constexpr auto children     = nullptr;
    constexpr auto help_filter  = nullptr;
    constexpr auto argp_domain  = nullptr;
    constexpr auto flags        = 0;
    constexpr auto end_index    = nullptr;

    static argp const argp
    {
        options,
        parser,
        args_doc,
        doc,
        children,
        help_filter,
        argp_domain
    };

    argp_parse( &argp, argc, argv, flags, end_index, this );
}

//----------------------------------------------------------------------------

error_t Options::parser( int const key, char* const arg, argp_state* const state )
{
    return static_cast< Options* >( state->input )->option( key, arg, state );
}

//----------------------------------------------------------------------------

error_t Options::option( int const key, char* const arg, argp_state* const state )
{
    auto const arg_num = state->arg_num;

    switch ( key )
    {
    case 'h': dump_html         = true;         break;
    case 'x': use_colour        = true;         break;
    case 'd': show_diagnostics  = true;         break;
    case 'v': logger_ptr        = &std::clog;   break;

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

//============================================================================
