#ifndef INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
#define INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP

#include <gtkmm.h>          // Gtk::Widget{}
#include <webkit/webkit.h>  // WebKitWebView{}

#include <string>           // std::string{}

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

    bool can_go_back();
    bool can_go_forward();

    void go_back();
    void go_forward();

    WebKitFindController* get_find_controller();

private:

    operator WebKitWebView*();
};

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================

#endif // INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
