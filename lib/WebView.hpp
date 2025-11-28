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

    void load_html( std::string const& content );

    bool can_go_back();
    bool can_go_forward();

    void go_back();
    void go_forward();

    bool is_loading();
    std::string get_uri();
    WebKitFindController* get_find_controller();

    virtual void on_load_changed( WebKitLoadEvent load_event );

private:

    operator WebKitWebView*();

    std::string root_html;

    void display_root_document();

    // signal handlers:

    static void load_changed_handler( WebKitWebView* self,
                                      WebKitLoadEvent load_event,
                                      void* user_data );
};

//----------------------------------------------------------------------------
} // close namespace WebKit
//============================================================================

#endif // INCLUDED_MDVIEW_WEBVIEW_WIDGET_HPP
