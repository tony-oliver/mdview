#include "Options.hpp"
#include "NullStream.hpp"

#include <iostream>
#include <streambuf>

//============================================================================

Options::Options( int const argc, char** const argv )
: logger_ptr{ &get_nullstream() }
{
    setenv( "ARGP_HELP_FMT", "header-col=2", true );

    constexpr argp_option options[]
    {
        { {},               {},  {}, {}, "Informational options:",                  {} },
        { "html",           'h', {}, {}, "Dump generated HTML to stdout",           {} },

        { {},               {},  {}, {}, "Developer options:",                      {} },
        { "verbose",        'v', {}, {}, "Produce verbose output on stderr",        {} },
        { "diagnostics",    'd', {}, {}, "Show LibTidy diagnostics on stderr",      {} },
        { "colour",         'c', {}, {}, "Distinguish verbose output by colours",   {} },

        { {},               {},  {}, {}, "General options:",                        -1 },
        {}
    };

    constexpr char args_doc[]   // description of non-option arguments
    {
        "MDFILE"
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
        "  MDFILE                     File containing markdown-text"
    };

    static argp const argp
    {
        options,
        parser,
        args_doc,
        doc,
        {},     // children,
        {},     // help_filter,
        {},     // argp_domain
    };

    argp_parse( &argp,
                argc,
                argv,
                {},     // flags
                {},     // arg_index
                this    // input
                );
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
    case 'c': use_colour        = true;         break;
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
