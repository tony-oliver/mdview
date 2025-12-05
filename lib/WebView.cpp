#include "WebView.hpp"

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

std::string const WebView::root_doc_uri{ "about:blank" };

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

void WebView::load_html( std::string const& content )
{
    std::lock_guard const locker( doc_mutex );

    root_html = content;

    auto const uri = get_uri();

    if ( uri.empty() || ( get_uri() == root_doc_uri ) )
    {
        display_root_document();
    }
}

//----------------------------------------------------------------------------

void WebView::display_root_document()
{
    std::lock_guard const locker( doc_mutex );

    webkit_web_view_load_html( *this, root_html.c_str(), nullptr );
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
        if ( get_uri() != root_doc_uri )
        {
            display_root_document();
        }
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

bool WebView::is_loading()
{
    return webkit_web_view_is_loading( *this );
}

//----------------------------------------------------------------------------

std::string WebView::get_uri()
{
    std::string uri;

    auto const uri_ptr = webkit_web_view_get_uri( *this );

    if ( uri_ptr != nullptr ) uri.assign( uri_ptr );

    return uri;
}

//----------------------------------------------------------------------------

WebKitFindController* WebView::get_find_controller()
{
    return webkit_web_view_get_find_controller( *this );
}

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
