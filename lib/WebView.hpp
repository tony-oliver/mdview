#ifndef INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
#define INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP

#include <gtkmm.h>          // Gtk::Widget{}
#include <webkit/webkit.h>  // WebKitWebView{}

#include <mutex>            // std::recursive_mutex{}
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

    void load_html( std::string const& content );
    void display_root_document();

    bool can_go_back();
    bool can_go_forward();

    void go_back();
    void go_forward();

    bool is_loading();
    std::string get_uri();
    WebKitFindController* get_find_controller();

protected:

    static std::string const root_doc_uri;

private:

    operator WebKitWebView*();

    std::recursive_mutex doc_mutex;
    std::string root_html;
};

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================

#endif // INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
