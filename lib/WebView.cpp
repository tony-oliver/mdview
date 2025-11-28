#include "WebView.hpp"

#include <magic_enum/magic_enum.hpp>

// WebKitWebView signals:
//
// notify::is-loading
// load-changed (WEBKIT_LOAD_STARTED)
// load-changed (WEBKIT_LOAD_COMMITTED)
// load-failed
// load-changed (WEBKIT_LOAD_FINISHED)
// load-changed (WEBKIT_LOAD_REDIRECTED)
// load-failed-with-tls-error
// notify::uri

/*
#define g_signal_connect(instance, detailed_signal, c_handler, data) \
    g_signal_connect_data ((instance), (detailed_signal), (c_handler), (data), NULL, (GConnectFlags) 0)

typedef void ( *GCallback )( void );

Use G_CALLBACK() to cast the callback function to a #GCallback.

unsigned long g_signal_connect( void*       instance,
                                char const* signal_name,
                                GCallback   c_handler,
                                void*       data )
*/

// DEBUG!!!
#include <cassert>
#include <iomanip>
#include <ostream>
#include <iostream>

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

WebView::WebView()
: Gtk::Widget{ webkit_web_view_new() }
{
    WebKitWebView* const signal_source = *this;
    constexpr auto signal_name = "load-changed";
    auto const signal_handler = G_CALLBACK( load_changed_handler );
    auto const user_data = this;

    g_signal_connect( signal_source, signal_name, signal_handler, user_data );
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
        display_root_document();
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

//----------------------------------------------------------------------------

void WebView::load_changed_handler( WebKitWebView* /* self */,
                                    WebKitLoadEvent const load_event,
                                    void* const user_data )
{
    static_cast< WebKit::WebView* >( user_data )->on_load_changed( load_event );
}

//----------------------------------------------------------------------------

void WebView::on_load_changed( WebKitLoadEvent const load_event )
{
    if ( load_event == WEBKIT_LOAD_STARTED ) std::clog << std::endl;

    std::clog << __FUNCTION__ << "( load_event = " << magic_enum::enum_name( load_event ) << " )";

    auto const loading = is_loading();

    assert( loading == ( load_event != WEBKIT_LOAD_FINISHED ) );

    if ( load_event != WEBKIT_LOAD_STARTED )
    {
        std::clog << ": uri = " << std::quoted( get_uri() );
    }

    std::clog << std::endl;
}

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================
