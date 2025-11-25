#ifndef WEBKIT_FIND_CONTROLLER_HPP
#define WEBKIT_FIND_CONTROLLER_HPP

#include <webkit/webkit.h>  // WebKitFindController{}

#include <limits>       // std::numeric_limits{}
#include <string>       // std::string{}

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

class FindController
{
    WebKitFindController* find_controller;

    static constexpr unsigned limitless_search = std::numeric_limits< unsigned >::max();

public:

    FindController( WebKitFindController* find_controller );

    void count_matches( std::string const& search_text,
                        WebKitFindOptions find_options,
                        unsigned max_match_count = limitless_search );

    void search( std::string const& search_text,
                 WebKitFindOptions find_options,
                 unsigned max_match_count = limitless_search );

    std::string get_search_text() const;
    WebKitFindOptions get_options() const;
    unsigned get_max_match_count() const;

    void search_next();
    void search_previous();

    void search_finish();

    // Signal handlers

    static void counted_matches(        WebKitFindController* self,
                                        unsigned match_count,
                                        void* user_data );

    static void found_text(             WebKitFindController* self,
                                        unsigned match_count,
                                        void* user_data );

    static void failed_to_find_text(    WebKitFindController* self,
                                        void* user_data );
};

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================

#endif // WEBKIT_FIND_CONTROLLER_HPP
