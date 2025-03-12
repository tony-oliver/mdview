project( mdview-libraries )

add_library( 	sundown
				sundown/src/autolink.c
				sundown/src/markdown.c
				sundown/src/buffer.c
				sundown/src/stack.c
				sundown/html/html.c
				sundown/html/houdini_html_e.c
				sundown/html/houdini_href_e.c
				)

add_subdirectory( md2html )
