#ifndef INCLUDED_AWO_THREAD_LOOP_HPP
#define INCLUDED_AWO_THREAD_LOOP_HPP

#include <map>          // std::map<>{}
#include <array>        // std::array<>{}
#include <mutex>        // std::mutex{}
#include <atomic>       // std::atomic_bool
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <ostream>      // std::ostream<>{}
#include <cstdint>      // std::intmax_t
#include <utility>      // std::pair<>{}
#include <functional>   // std::function<>{}

namespace awo {

class ThreadLoop
{
    using Action = std::function< void() >;

public:

    explicit ThreadLoop( std::ostream& logger );
    ~ThreadLoop();

    void start();
    void stop();

protected:

    std::ostream& logger;

    void registerActionForFD( int fd, Action const& action );
    void deregisterFD( int fd );

    bool checkForPosixError( std::intmax_t value, std::string const& what );

private:

    std::map< int, Action > action_table;
    std::recursive_mutex action_table_mutex; // action_table is accessed from our thread and the host
    void executeActionForFD( int fd );

    enum PipeFDTypes { ReadEnd, WriteEnd, NumPipeFDs };
    using PipeFDs = std::array< int, NumPipeFDs >;
    PipeFDs stop_fds;

    std::atomic_bool stopping = false;
    std::thread polling_thread;
    void pollingLoop();
};

} // close namespace awo

#endif // INCLUDED_AWO_THREAD_LOOP_HPP
