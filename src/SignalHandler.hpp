#ifndef INCLUDED_AWO_SIGNAL_HANDLER_HPP
#define INCLUDED_AWO_SIGNAL_HANDLER_HPP

#include <set>
#include <functional>

namespace awo {

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

} // close namespace awo

#endif // INCLUDED_AWO_SIGNAL_HANDLER_HPP
