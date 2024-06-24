#ifndef INCLUDED_THREAD_SAFE_OUTPUT_STREAM_BUFFER_HPP
#define INCLUDED_THREAD_SAFE_OUTPUT_STREAM_BUFFER_HPP

#include <ios>          // std::streamsize
#include <map>          // std::map<>{}
#include <mutex>        // std::mutex{}
#include <string>       // std::string{}
#include <thread>       // std::thread{}
#include <ostream>      // std::ostream{}
#include <streambuf>    // std::streambuf{}

class ThreadSafeOStream: public std::streambuf
{
public:

    explicit ThreadSafeOStream( std::ostream& stream, bool use_colours = false );
    virtual ~ThreadSafeOStream();

private:

    // The overriden virtual functions that make this streambuf thread-safe

    virtual int_type overflow( int_type ch ) final;
    virtual std::streamsize xsputn( char const* s, std::streamsize count ) final;

    // Saved constructor parameters that can be restored on destruction

    std::ostream& host_ostream;
    std::streambuf* const underlying_streambuf;

    // Logging and colour management (when enabled and permitted)

    bool const using_colours = false;
    std::map< std::thread::id, std::size_t > colours_in_use;
    std::size_t get_colour_no_for( std::thread::id thread_id );

    // The thread-specific text buffers and helper function(s)

    std::mutex buffers_mutex;
    std::map< std::thread::id, std::string > buffers;
    void flushBuffer( std::thread::id thread_id, std::string& buffer, std::streamsize length );
};

#endif // INCLUDED_THREAD_SAFE_OUTPUT_STREAM_BUFFER_HPP
