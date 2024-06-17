#ifndef INCLUDED_AWO_THREAD_LOOP_HPP
#define INCLUDED_AWO_THREAD_LOOP_HPP

#include <map>          // std::map<>{}
#include <array>        // std::array<>{}
#include <mutex>        // std::mutex{}
#include <atomic>       // std::atomic_bool
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <cstdint>      // std::maxint_t
#include <utility>      // std::pair<>{}
#include <functional>   // std::function<>{}

class ThreadLoop
{
    using Action = std::function< void() >;

public:

    ThreadLoop();
    ~ThreadLoop();

    void start();
    void stop();

protected:

    void registerActionForFD( int fd, Action const& action );
    void deregisterFD( int fd );

    bool checkForPosixError( std::intmax_t value, std::string const& what );

    // Provision to convert int bitmasks to strings of manifest constant name(s)
    using Mappings = std::vector< std::pair< int, std::string > >;
    std::string bitmaskToString( int bitmask, Mappings const& mappings );

private:

    std::map< int, Action > action_table;
    std::recursive_mutex action_table_mutex; // action_table is accessed from our thread and the host
    void executeActionForFD( int fd );

    enum PipeFDTypes { ReadEnd, WriteEnd, NumPipeFDs };
    using PipeFDs = std::array< int, NumPipeFDs >;
    PipeFDs stop_fds;
    void clearPipe();

    std::atomic_bool stopping = false;
    std::thread polling_thread;
    void pollingLoop();

    // Conversion to string of 'events' / 'revents' fields of pollfd structures
    std::string eventsToString( int value );
};

#endif // INCLUDED_AWO_THREAD_LOOP_HPP
