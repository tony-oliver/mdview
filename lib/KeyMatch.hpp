#ifndef INCLUDED_KEY_MATCH_HPP
#define INCLUDED_KEY_MATCH_HPP

#include <gtkmm.h>

#include <functional>

struct KeyMatch
{
    unsigned keyval;
    Gdk::ModifierType state;
    std::function< void() > action;
};

bool match_key( unsigned keyval, Gdk::ModifierType state, KeyMatch const* matches );

#endif // INCLUDED_KEY_MATCH_HPP
