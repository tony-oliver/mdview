#ifndef INCLUDED_HTML_RENDERER_HPP
#define INCLUDED_HTML_RENDERER_HPP

#include "markdown.h"   // mkd_extensions
#include "html.h"       // html_render_mode

#include <string>       // std::string{}

//============================================================================

class HTMLRenderer
{
    sd_callbacks html_callbacks;
    html_renderopt html_options;

public:

    HTMLRenderer( html_render_mode render_flags = {} );

    std::string render( std::string const& markdown_text );
};

//============================================================================

#endif // INCLUDED_HTML_RENDERER_HPP
