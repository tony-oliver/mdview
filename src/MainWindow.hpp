#ifndef INCLUDED_MDVIEW_MAIN_WINDOW_HPP
#define INCLUDED_MDVIEW_MAIN_WINDOW_HPP

#include "WebView.hpp"

#include <gtkmm/window.h>

#include <string>
#include <vector>
#include <cstddef>
#include <string_view>

class MainWindow: public Gtk::Window
{
public:

    using Arguments = std::vector< std::string >;

    MainWindow( Arguments const& args );

private:

    WebView webView;
};

#endif // INCLUDED_MDVIEW_MAIN_WINDOW_HPP
