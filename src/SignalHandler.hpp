#ifndef INCLUDED_AWO_SIGNAL_HANDLER_HPP
#define INCLUDED_AWO_SIGNAL_HANDLER_HPP

#include <set>
#include <functional>

class SignalHandler
{
public:

    using Action = std::function< void() >;

    explicit SignalHandler( std::set< int > const& signos );

    void registerAction( Action const& newAction );

private:

    static inline Action action;

    static void handleSignal( int signo );
};

#endif // INCLUDED_AWO_SIGNAL_HANDLER_HPP
