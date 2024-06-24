#ifndef INCLUDED_MDVIEW_FILE_WATCHER_HPP
#define INCLUDED_MDVIEW_FILE_WATCHER_HPP

#include "ThreadLoop.hpp"
#include "Options.hpp"

#include <map>          // std::map<>{}
#include <array>        // std::array<>{}
#include <atomic>       // std::atomic_bool
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <utility>      // std::pair<>{}
#include <functional>   // std::function<>{}

class FileWatcher: public awo::ThreadLoop
{
public:

    explicit FileWatcher( Options const& options );
    ~FileWatcher();

    using Action = std::function< void() >;

    void watchFile( std::string const& filename, Action const& action );
    void ignoreFile( std::string const& filename );
    bool isWatching( std::string const& filename );

private:

    int const inotify_fd;

    using WDAction = std::pair< int, Action >;

    void handleINotification();

    std::map< std::string, WDAction > file_watchers;
    std::recursive_mutex file_watchers_mutex; // file_watchers is accessed from our thread and the host
    void executeActionForWD( int event_wd );
};

#endif // INCLUDED_MDVIEW_FILE_WATCHER_HPP
