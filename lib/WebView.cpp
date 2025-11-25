#include "WebView.hpp"

// DEBUG!!!
#include <iomanip>
#include <ostream>
#include <iostream>

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

WebView::WebView()
: Gtk::Widget{ webkit_web_view_new() }
{
}

//----------------------------------------------------------------------------

WebView::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW( gobj() );
}

//----------------------------------------------------------------------------

void WebView::load_html( std::string const& content, std::string const& base_uri )
{
    webkit_web_view_load_html( *this, content.c_str(), base_uri.c_str() );
}

//----------------------------------------------------------------------------

bool WebView::can_go_back()
{
    auto const ok = webkit_web_view_can_go_back( *this );

    std::clog << "can_go_back = " << std::boolalpha << ok << std::endl;

    return ok;
}

//----------------------------------------------------------------------------

bool WebView::can_go_forward()
{
    auto const ok = webkit_web_view_can_go_forward( *this );

    std::clog << "can_go_forward = " << std::boolalpha << ok << std::endl;

    return ok;
}

//----------------------------------------------------------------------------

void WebView::go_back()
{
    if ( can_go_back() )
    {
        webkit_web_view_go_back( *this );
    }
}

//----------------------------------------------------------------------------

void WebView::go_forward()
{
    if ( can_go_forward() )
    {
        webkit_web_view_go_forward( *this );
    }
}

//----------------------------------------------------------------------------

WebKitFindController* WebView::get_find_controller()
{
    return webkit_web_view_get_find_controller( *this );
}

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
