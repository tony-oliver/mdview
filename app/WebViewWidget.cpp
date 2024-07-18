#include "WebViewWidget.hpp"

WebViewWidget::WebViewWidget()
: Gtk::Widget{ webkit_web_view_new() }
{
}

void WebViewWidget::load_html( std::string const& content, std::string const& base_uri )
{
    webkit_web_view_load_html( *this, content.c_str(), base_uri.c_str() );
}

WebViewWidget::operator WebKitWebView*()
{
    return WEBKIT_WEB_VIEW( gobj() );
}
