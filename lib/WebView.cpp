#include "WebView.hpp"

#include <magic_enum/magic_enum.hpp>

#include <cstdint>

//#define LOG_LOAD_CHANGED_EVENTS

#ifdef LOG_LOAD_CHANGED_EVENTS
#include <iomanip>
#include <ostream>
#include <iostream>
#endif

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

std::string const WebView::root_doc_uri{ "about:blank" };

//----------------------------------------------------------------------------

WebView::WebView()
: Gtk::Widget{ webkit_web_view_new() }
{
    g_signal_connect( *this,
                      "load-changed",
                      G_CALLBACK( load_changed_handler ),
                      this );

}

//----------------------------------------------------------------------------

WebView::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW( gobj() );
}

//----------------------------------------------------------------------------

void WebView::load_html( std::string const& content )
{
    root_html = content;

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

void WebView::display_root_document()
{
    webkit_web_view_load_html( *this, root_html.c_str(), nullptr );
}

//============================================================================

void WebView::load_changed_handler( WebKitWebView*  /* const self */,
                                    WebKitLoadEvent const load_event,
                                    void*           const user_data )
{
    static_cast< WebView* >( user_data )->on_load_changed( load_event );
}

//----------------------------------------------------------------------------

void WebView::on_load_changed( [[maybe_unused]] WebKitLoadEvent const load_event )
{
#ifdef LOG_LOAD_CHANGED_EVENTS
    if ( load_event == WEBKIT_LOAD_STARTED ) std::clog << std::endl;

    std::clog   << __FUNCTION__
                << "( load_event = " << magic_enum::enum_name( load_event ) << " )"
                << ": uri = " << std::quoted( get_uri() ) << std::endl;
#endif
}
//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
