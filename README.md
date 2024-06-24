# mdview: a markdown-file viewer

## Description

`mdview` is a program for viewing markdown files (conventionally denoted by the `.md` suffix in their filenames).

Once installed, it is invoked thus:

```
$ mdview <filename>
```

which parses the contents of the named file from markdown to HTML and displays the latter in an HTML widget.

It also watches for file-content updates and keeps the displayed image in sync when they occur.

## Background

Some few years ago, as `github` become immensely popular, I found that project sources (either git repos or
tarballs) carried a `README.md` file, instead of the traditional plaintext `README` file.

It frustrated me that there was no standard tool to render and display such markdown files (apart from editors,
clumsily showing the rendered version side-by-side with the marked-down source text).

Finally, I got around to developing such a tool - this program, `mdview`.

This was an ideal hobby project on which to learn the features and limitations of `gtkmm-4`, having previously used `gtkmm-3`
on my last C++ GUI-based projects. It also gave me a good look at how `inotify` monitors and reports changes to files.

## Packaging

This project is available as a source tree in its own `git` repository.

It has been tested (so far) on the following platform(s):

* Fedora 39 (using `g++ v13`).

See the **Building** section, below, for the location of that repository.

## Dependencies

### Compiler

The default C++ compiler must be capable of supporting the C++20 standard.

### Packages

This program uses the `gtkmm-4.0` package for its windowing framework, 

For the HTML-viewing widget in that framework, it uses the WebKit-GTK bindings.

For the markdown-to-HTML parser it uses the `cmark` package.

Installation of these packages is as simple as

```
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel cmark-devel
```

(For other flavours of Linux, the appropriate package management tool should
be used instead of `dnf`, *e.g.* `yum`, `apt`, `pkg` *etc.*).

## Building

### Detailed

Firstly, download a copy of the repo and navigate into its source root:

```
$ git clone https://github.com/tony-oliver/mdview && cd mdview
```

The top-level Makefile is used for co-ordinating the build process:

```
$ make
```

or

```
$ make all
```

Simple testing (displaying this `README.md` file) can be run as a confidence test after building:

```
$ make run
```

Installation is accomplished with

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
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel cmark-devel
$ git clone https://github.com/tony-oliver/mdview && cd mdview
$ make install
$ cd .. && rm -rf mdview
```

(Again, use `apt`, `yum`, `pkg`, *etc.* instead of `dnf`, according to the
package manager installed for your particular flavour of Linux.)
