#include "FindController.hpp"

#include <stdexcept>    // std::runtime_error{}

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

FindController::FindController( WebKitFindController* find_controller )
: find_controller{ find_controller }
{
}

//----------------------------------------------------------------------------

void FindController::search( std::string const& search_text,
                             WebKitFindOptions const find_options,
                             unsigned const max_match_count )
{
    webkit_find_controller_search( find_controller, search_text.c_str(),
                                   find_options, max_match_count );
}

//----------------------------------------------------------------------------

std::string FindController::get_search_text() const
{
    auto const search_text = webkit_find_controller_get_search_text( find_controller );

    if ( search_text == nullptr )
    {
        return {};
    }

    return search_text;
}

//----------------------------------------------------------------------------

WebKitFindOptions FindController::get_options() const
{
    return static_cast< WebKitFindOptions >( webkit_find_controller_get_options( find_controller ) );
}

//----------------------------------------------------------------------------

unsigned FindController::get_max_match_count() const
{
    return webkit_find_controller_get_max_match_count( find_controller );
}

//----------------------------------------------------------------------------

void FindController::search_next()
{
    webkit_find_controller_search_next( find_controller );
}

//----------------------------------------------------------------------------

void FindController::search_previous()
{
    webkit_find_controller_search_previous( find_controller );
}

//----------------------------------------------------------------------------

void FindController::search_finish()
{
    webkit_find_controller_search_finish( find_controller );
}

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
