#include "KeyMatch.hpp"

bool match_key( unsigned const keyval, Gdk::ModifierType const state, KeyMatch const* const matches )
{
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
