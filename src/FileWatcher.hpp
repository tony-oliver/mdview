#ifndef INCLUDED_MDVIEW_FILE_WATCHER_HPP
#define INCLUDED_MDVIEW_FILE_WATCHER_HPP

#include "ThreadLoop.hpp"

#include <map>          // std::map<>{}
#include <array>        // std::array<>{}
#include <atomic>       // std::atomic_bool
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <utility>      // std::pair<>{}
#include <functional>   // std::function<>{}

class FileWatcher: public ThreadLoop
{
public:

    FileWatcher();
    ~FileWatcher();

    using Action = std::function< void() >;

    void watchFile( std::string const& filename, Action const& action );
    void ignoreFile( std::string const& filename );

protected:

    void handleINotification();

private:

    int const inotify_fd;

    using WDAction = std::pair< int, Action >;

    std::map< std::string, WDAction > file_watchers;
    std::recursive_mutex file_watchers_mutex; // file_watchers is accessed from our thread and the host
    void executeActionForWD( int event_wd );

    std::string event_mask_to_string( int event_mask );
};

#endif // INCLUDED_MDVIEW_FILE_WATCHER_HPP
