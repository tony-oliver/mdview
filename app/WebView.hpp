#ifndef INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
#define INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP

#include <gtkmm/widget.h>   // for Gtk::Widget{}
#include <webkit/webkit.h>  // for WebKitWebView{}

#include <string>           // for std::string{}

//============================================================================
namespace WebKit {
//----------------------------------------------------------------------------

class WebView: public Gtk::Widget
{
public:

    WebView();
    virtual ~WebView() = default;

    // Functionality wrapping webkit_web_view_XXX() functions:

    void load_html( std::string const& content,
                    std::string const& base_uri = {} );

private:

    operator WebKitWebView*();
};

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================

#endif // INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
