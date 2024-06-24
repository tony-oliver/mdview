#ifndef INCLUDED_MDVIEW_WEBVIEW_HPP
#define INCLUDED_MDVIEW_WEBVIEW_HPP

#include <gtkmm/widget.h>
#include <webkit/webkit.h>

#include <string>

class WebView: public Gtk::Widget
{
    operator WebKitWebView*();

public:

    WebView();
    virtual ~WebView() = default;

    // Functionality wrapping webkit_web_view_XXX() functions:

    void load_html( std::string const& content,
                    std::string const& base_uri = {} );
};

#endif // INCLUDED_MDVIEW_WEBVIEW_HPP
