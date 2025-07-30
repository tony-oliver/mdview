# mdview: a markdown-file viewer

## Description

`mdview` is a program for viewing markdown files (conventionally denoted by the `.md` suffix in their filenames).

Once installed on your system, it is invoked thus:

```
$ mdview <filename>
```

which parses the (markdown) content of the named file, converts it to HTML and displays the latter in an HTML widget.

It also watches for markdown file-contents updates and keeps the rendered HTML in sync whenever they occur.

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

It has been built and tested (so far) on the following platform(s):

* Fedora 39 (built using `g++ v13`).
* Fedora 41 (built using `g++ v14`).
* Fedora 42 (built using `g++ v15`).

## Dependencies

### Compiler

The default C++ compiler must be capable of supporting the C++20 standard
(through the options ``-std=c++20 -pedantic``).

### Packages

This program uses the `gtkmm-4.0` package for its windowing framework (which wraps the C-only `GTK4+` package into C++ classes). 

For an HTML-viewing widget within the `GTK4+` framework, we use the WebKit-GTK bindings.

The original choice for the Markdown-to-HTML converter was to use the [cmark](https://github.com/commonmark/cmark) package
(the reference implementation of a [CommonMark](https://commonmark.org) converter).
Unfortunately, many markdown files are authored using certain *de facto* markdown extensions (mainly parts of Github Markdown, *e.g.*
[tables](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/organizing-information-with-tables))
that are not covered in Common Markdown.
Finally, the `md4c` package was settled on (which can easily be configured to operate in Github Markdown mode) for the job;
it is also reputed to be significantly faster than other such libraries.

For tidying-up the generated HTML, `libtidy` gets the job.

Installation of the shared-library packages, above, is as simple as

```
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel md4c-devel libtidy-devel
```

(For Linux flavours other than Fedora, the appropriate package management tool should be used
instead of `dnf`, *e.g.* `yum`, `apt`, `pkg`, *etc.*).

Also note that some package names will end `-dev` (instead of `-devel`) when using certain
non-`dnf` package managers (*e.g.* `apt`).

Here are the versions of these external components that `mdview` was originally built against:

| Component 		| Description 					| Version
| ----------------: | :---------------------------- | :-----: 
| gtkmm4.0-devel	| GTKmm GUI framework			| 4.16.0
| webkit6.0-devel 	| HTML-rendering GTK+ widget	| 2.46.5
| md4c-devel 		| Markdown-to-HTML converter	| 0.5.1
| libtidy-devel 	| HTML fixer and reformatter	| 5.8.0

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

which will create the executable `build/app/mdview`.

---

Simple testing (displaying this `README.md` file) can be run as a confidence test after building:

```
$ make run
```

In particular, we expect *tables* to be rendered correctly, such as the one
in the Packages section, above.

---

Installation (to `/usr/local`) is accomplished with

```
$ make install
```

and uninstallation with

```
$ make uninstall
```

*(provided the project has not been cleaned; see below).*
 
---

Project cleanup can be performed with

```
$ make clean
```

### Quick build

```
$ pushd /tmp
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel md4c-devel libtidy-devel
$ git clone https://github.com/tony-oliver/mdview && cd mdview
$ make install
$ cd .. && rm -rf mdview
$ popd
```

(The notes in the `Packages` section, above, also apply here).

