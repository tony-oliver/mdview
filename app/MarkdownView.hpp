#ifndef INCLUDED_MARKDOWN_VIEW_HPP
#define INCLUDED_MARKDOWN_VIEW_HPP

#include "WebView.hpp"
#include "FileWatcher.hpp"

#include <string>       // std::string{}
#include <ostream>      // std::ostream{}

class MarkdownView: public WebKit::WebView
{
    bool const dump_html;
    bool const show_diagnostics;
    std::string const& filename;
    FileWatcher watcher;

public:

    MarkdownView(   std::ostream& logger,
                    std::string const& filename,
                    bool dump_html = false,
                    bool show_diagnostics = false );

    void render();

private:

    void postProcess( std::string& html );
};

#endif // INCLUDED_MARKDOWN_VIEW_HPP
