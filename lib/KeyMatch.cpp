#include "KeyMatch.hpp"

#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_flags.hpp>

#include <map>          // std::map<>{}
#include <ostream>      // std::to_underlying
#include <utility>      // std::ostream{}, std::endl()
#include <iostream>     // std::clog

//============================================================================
namespace { // unnamed
//----------------------------------------------------------------------------

std::string keyvalName( unsigned const keyval )
{
    static std::map< unsigned, std::string > const keyval_names
    {
        #define X( keyval, keyname ) { keyval, keyname },
        #include "gdk-keynames.x-macros"
        #undef X
    };

    auto const p = keyval_names.find( keyval );
    if ( p == keyval_names.cend() ) return {};
    return p->second;
}

//----------------------------------------------------------------------------

std::string modifierName( Gdk::ModifierType const modifier )
{
    auto name = magic_enum::enum_flags_name( modifier );
    if ( name.empty() ) name = magic_enum::enum_name( modifier );
    return std::string( name.cbegin(), name.cend() );
}

//----------------------------------------------------------------------------

void announce_key( std::string const& prefix, unsigned const keyval, Gdk::ModifierType const state )
{
    if ( !prefix.empty() ) std::clog << prefix << ": ";

    std::clog << "keyval = " << keyval;
    auto const keyval_name = keyvalName( keyval );
    if ( !keyval_name.empty() ) std::clog << " (" << keyval_name << ")";

    std::clog << ", ";

    std::clog << "state = " << std::to_underlying( state );
    auto const state_name = modifierName( state );
    if ( !state_name.empty() ) std::clog << " (" << state_name << ")";

    std::clog << std::endl;
}

//----------------------------------------------------------------------------
} // close unnamed namespace
//============================================================================

bool match_key( std::string const& prefix,
                unsigned const keyval,
                Gdk::ModifierType const state,
                KeyMatch const* const matches,
                bool const verbose )
{
    if ( verbose )
    {
        announce_key( prefix, keyval, state );
    }

    bool key_found = false;

    for ( auto match = matches; match->action; ++match )
    {
        key_found = ( keyval == match->keyval ) && ( state == match->state );

        if ( key_found )
        {
            match->action();
            break;
        }
    }

    return key_found;
}
