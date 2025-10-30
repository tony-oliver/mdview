#ifndef INCLUDED_AWO_SIGNAL_HANDLER_HPP
#define INCLUDED_AWO_SIGNAL_HANDLER_HPP

#include <set>
#include <functional>

namespace awo {

class SignalHandler
{
public:

    using Action = std::function< void() >;
    using Signals = std::set< int >;

    static Signals const default_signos; // { SIGINT, SIGTERM, SIGQUIT, SIGHUP, SIGPWR }

    explicit SignalHandler( Signals const& signos = default_signos );

    void registerAction( Action const& newAction );

private:

    static inline Action action;

    static void handleSignal( int signo );
};

} // close namespace awo

#endif // INCLUDED_AWO_SIGNAL_HANDLER_HPP
