#ifndef INCLUDED_MDVIEW_FILE_WATCHER_HPP
#define INCLUDED_MDVIEW_FILE_WATCHER_HPP

#include "Options.hpp"      // for Options{}
#include "ThreadLoop.hpp"   // for awo::ThreadLoop{}

#include <map>          // std::map<>{}
#include <array>        // std::array<>{}
#include <atomic>       // std::atomic_bool
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <utility>      // std::pair<>{}
#include <functional>   // std::function<>{}

class FileWatcher: public awo::ThreadLoop
{
    using Action = std::function< void() >;

public:

    explicit FileWatcher( std::ostream& logger );
    virtual ~FileWatcher();

    void watchFile( std::string const& filename, Action const& action );
    void ignoreFile( std::string const& filename );
    bool isWatching( std::string const& filename );

private:

    int const inotify_fd;

    using WD = int;
    using WDAction = std::pair< WD, Action >;
    std::map< std::string, WDAction > file_watchers;
    std::recursive_mutex file_watchers_mutex;

    void handleINotification();
    void executeActionForWD( int event_wd );
};

#endif // INCLUDED_MDVIEW_FILE_WATCHER_HPP
