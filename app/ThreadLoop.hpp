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
    virtual ~ThreadLoop();

    void registerActionForFD( int fd, Action const& action );
    void deregisterFD( int fd );

    void start();
    void stop();

protected:

    std::ostream& logger;

    bool checkForPosixError( std::intmax_t value, std::string const& what );

private:

    std::map< int, Action > action_table;
    std::recursive_mutex action_table_mutex;

    void executeActionForFD( int fd );

    enum PipeFDType { ReadEnd, WriteEnd, NumPipeFDs };
    using PipeFDs = std::array< int, NumPipeFDs >;
    PipeFDs stop_fds;

    std::atomic_bool stopping = false;
    std::thread polling_thread;

    void pollingLoop();
    void closeStopFD( PipeFDType end, std::string const& endName );
};

} // close namespace awo

#endif // INCLUDED_AWO_THREAD_LOOP_HPP
