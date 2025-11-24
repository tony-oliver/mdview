#include "WebView.hpp"

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

WebKitFindController* WebView::get_find_controller()
{
    return webkit_web_view_get_find_controller( WEBKIT_WEB_VIEW( gobj() ) );
}

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
