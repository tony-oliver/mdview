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
    root_html = content;
    root_uri = base_uri;

    display_root_document();
}

//----------------------------------------------------------------------------

bool WebView::can_go_back()
{
    return webkit_web_view_can_go_back( *this );
}

//----------------------------------------------------------------------------

bool WebView::can_go_forward()
{
    return webkit_web_view_can_go_forward( *this );
}

//----------------------------------------------------------------------------

void WebView::go_back()
{
    if ( can_go_back() )
    {
        webkit_web_view_go_back( *this );
    }
    else
    {
        display_root_document();
    }
}

//----------------------------------------------------------------------------

void WebView::display_root_document()
{
    webkit_web_view_load_html( *this, root_html.c_str(), root_uri.c_str() );
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
