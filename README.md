# mdview: a markdown-file viewer

## Description

`mdview` is a program for viewing markdown files (conventionally denoted by the `.md` suffix in their filenames).

Once installed on your system, it is invoked thus:

```
mdview <filename>
```

which parses the (markdown) content of the named file, converts it to HTML and displays the latter in an HTML widget.

It also watches for changes in the markdown file-contents and keeps the rendered HTML in sync whenever they occur.

## Background

Some few years ago, as [github](https://github.com) became immensely popular, I found that project sources
(either [git](https://git-scm.com) repositories or
tarballs) carried a `README.md` file, instead of the traditional ASCII `README` file.

It frustrated me that there was no standard tool to render and display such markdown files (apart from text editors,
clumsily showing the rendered version side-by-side with the marked-down source text).

Finally, I got around to developing such a tool: this program, `mdview`.

This was an ideal hobby project on which to learn the features and limitations of [gtkmm-4](https://gtkmm.gnome.org)
(having used only `gtkmm-3` on my previous C++ GUI-based projects).

It also gave me a good look at how [inotify](https://en.wikipedia.org/wiki/Inotify)
monitors (and reports) changes to files, as well as
how to make the displayed HTML use the visual styles adopted by GitHub and by markdown editors.

UPDATE: I've now added a search facility, using the `find_controller` facilities in `WebKitWebView`
and some fancy keypress-handling.

## Usage

| Key combination 			| Action 																|
| ------------------------- | --------------------------------------------------------------------- |
| PgUp/PgDown 				| Scroll through the document as expected.								|
| Home/End (*without* Ctrl) | Go to top/bottom of document. 										|
| ^F 						| Brings up *Search* dialog (in which Enter = find; Escape = close).	|
| F3 						| Find next.															|
| Shift+F3 					| Find previous.														|
| n 						| Find next.															|
| N 						| Find previous.														|
| Alt+n 					| Find next.															|
| Alt+N 					| Find previous.														|
| Escape 					| Clear highlighted search items.										|
| Alt+Left 					| Go back (after clicking hyperlink).									|
| Alt+Right 				| Go forward (when hyperlink remembered).								|

## Packaging

This project is available from its own `git` [repository](https://github.com/tony-oliver/mdview).

It has been built and tested (so far) on the following platform(s):

* Fedora 43 (built using `g++ v15.2.1` and `clang++ v21.1.5`).

## Dependencies

### Compiler

The default C++ compiler must be capable of supporting the C++23 standard, through the options `-std=c++23 -pedantic`.

### Packages

This program uses the `gtkmm-4.0` package for its windowing framework
(which wraps the C-only [GTK4+](https://docs.gtk.org/gtk4) package into C++ classes). 

For an HTML-viewing widget within the `GTK4+` framework, we use the WebKit-GTK 6.0 bindings.

The original choice for the Markdown-to-HTML converter was to use the [cmark](https://github.com/commonmark/cmark) package
(the reference implementation of a [CommonMark](https://commonmark.org) converter).
Unfortunately, many markdown files are authored using certain *de facto* markdown extensions (mainly parts of Github Markdown, *e.g.*
[tables](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/organizing-information-with-tables))
that are not covered in Common Markdown.
So, eventually, the [md4c](https://github.com/mity/md4c) package was settled on (which can easily be configured to operate in Github Markdown mode).

For tidying-up the generated HTML, [libtidy](https://www.html-tidy.org/developer) gets the job,
and for preparing enum-names for debug-output, [magic_enum](https://github.com/Neargye/magic_enum) is used.

The Terminfo library (installed as part of the [ncurses](https://invisible-island.net/ncurses) package)
is employed to colour-code different threads' verbose output.

Under Fedora, installation of these packages is as simple as

```
sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel md4c-devel libtidy-devel magic_enum-devel ncurses-devel
```

(Note: for Linux flavours other than Fedora, the appropriate package management tool should be used
instead of `dnf`, *e.g.* `yum`, `apt`, `pkg`, *etc.*).

Also note that some package names will end `-dev` (instead of `-devel`) when using certain
non-`dnf` package managers (*e.g.* `apt`).

Here are the versions of these external components that `mdview` was originally built against:

| Component 			| Description 					| Original 	| Current
| --------------------: | :---------------------------- | :-------: | :-----: 
| `gtkmm4.0-devel`	| GTKmm GUI framework			| 4.16.0	| 4.20.0
| `webkit6.0-devel`	| HTML-rendering GTK+ widget	| 2.46.5	| 2.50.1
| `md4c-devel`			| Markdown-to-HTML converter	| 0.5.1		| 0.5.2
| `libtidy-devel`	 	| HTML fixer and reformatter	| 5.6.0		| 5.8.0
| `magic_enum-devel` 	| C++ enum introspection		| 0.9.7		| 0.9.7
| `ncurses-devel`		| Character terminal control	| 6.4		| 6.5

## Building

### Detailed

Firstly, download a copy of the repo and navigate into its source root:

```
git clone https://github.com/tony-oliver/mdview && cd mdview
```

The top-level Makefile is used for co-ordinating the build process, thus:

```
make
```

or

```
make all
```

which will create the executable `build/app/mdview`.

---

Simple testing (displaying this `README.md` file) can be run as a confidence test after building:

```
make run
```

In particular, we expect *tables* to be rendered correctly, such as the one
in the Packages section, above.

---

Installation (to `/usr/local`) is accomplished with

```
make install
```

and uninstallation with

```
make uninstall
```

*(provided the project has not been 'cleaned'; see below).*
 
---

Project cleanup can be performed with

```
make clean
```

### Quick build

```
sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel md4c-devel libtidy-devel magic_enum-devel ncurses-devel
pushd /tmp >/dev/null
git clone https://github.com/tony-oliver/mdview && cd mdview
make install
cd .. && rm -rf mdview
popd >/dev/null
```

(The notes in the `Packages` section, above, also apply here).
