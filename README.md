# mdview: a markdown-file viewer

## Description

`mdview` is a program for viewing markdown files (conventionally denoted by the `.md` suffix in their filenames).

Once installed, it is invoked thus:

```
$ mdview <filename>
```

which parses the contents of the named file from markdown, converts it to HTML and displays the latter in an HTML widget.

It also watches for file-content updates and keeps the rendered content in sync whenever they occur.

## Background

Some few years ago, as `github` became immensely popular, I found that project sources (either `git` repositories or
tarballs) carried a `README.md` file, instead of the traditional ASCII `README` file.

It frustrated me that there was no standard tool to render and display such markdown files (apart from text editors,
clumsily showing the rendered version side-by-side with the marked-down source text).

Finally, I got around to developing such a tool: this program, `mdview`.

This was an ideal hobby project on which to learn the features and limitations of `gtkmm-4`
(having used only `gtkmm-3` on my previoius C++ GUI-based projects).
It also gave me a good look at how `inotify` monitors (and reports) changes to files, as well as
how to make the displayed HTML use the visual styles adopted by GitHub and by markdown editors.

## Packaging

This project is available from its own `git` [repository](https://github.com/tony-oliver/mdview).

It has been tested (so far) on the following platform(s):

* Fedora 39 (built using `g++ v13`).
* Fedora 41 (built using `g++ v14`).

## Dependencies

### Compiler

The default C++ compiler must be capable of supporting the C++20 standard
(through the options ``-std=c++20 -pedantic``).

### Packages

This program uses the `gtkmm-4.0` package for its windowing framework (which wraps the C-only `GTK 4` package). 

For the HTML-viewing widget within the `GTK 4` framework, it uses the WebKit-GTK bindings.

For the markdown-to-HTML parser it uses the `cmark` package; this only parses Common Markdown, which does not
implement some of the extensions provided by other "standards" (*e.g.* GitHub Markdown).

For tidying-up the generated HTML, `libtidy` gets the job.

Installation of these packages is as simple as

```
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel cmark-devel libtidy-devel
```

(For other flavours of Linux, the appropriate package management tool should be used
instead of `dnf`, *e.g.* `yum`, `apt`, `pkg`, *etc.*).

Also note that some package names will end `-dev` (instead of `-devel`) when using certain
non-`dnf` package managers (*e.g.* `apt`).

## Building

### Detailed

Firstly, download a copy of the repo and navigate into its source root:

```
$ git clone https://github.com/tony-oliver/mdview && cd mdview
```

The top-level Makefile is used for co-ordinating the build process, thus:

```
$ make
```

or

```
$ make all
```

will create the executable `build/src/mdview`.

Simple testing (displaying this `README.md` file) can be run as a confidence test after building:

```
$ make run
```

Installation (to `/usr/local`) is accomplished with

```
$ make install
```

and uninstallation with

```
$ make uninstall
```

*(provided the project has not been cleaned; see below).*
 
Project cleanup can be performed with

```
$ make clean
```

### Quick build

```
$ pushd /tmp
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel cmark-devel libtidy-devel
$ git clone https://github.com/tony-oliver/mdview && cd mdview
$ make install
$ cd .. && rm -rf mdview
$ popd
```

(The notes in the `Packages` section, above, also apply here).
