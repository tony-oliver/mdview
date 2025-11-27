#ifndef INCLUDED_KEY_MATCH_HPP
#define INCLUDED_KEY_MATCH_HPP

#include <gtkmm.h>

#include <string>
#include <functional>

struct KeyMatch
{
    unsigned keyval;
    Gdk::ModifierType state;
    std::function< void() > action;
};

bool match_key( std::string const& prefix, unsigned keyval, Gdk::ModifierType state, KeyMatch const* matches );

#endif // INCLUDED_KEY_MATCH_HPP
