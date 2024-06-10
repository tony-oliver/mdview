# mdview - a markdown-file viewer

## Description

`mdview` is a program to view markdown files (usually denoted by
the `.md` extension in their filenames). Once installed, it is
invoked thus:

``` console
$ mdview <filename>
```

which parses the markdown content to HTML and displays it.

## Packaging

This project is available as a source tree in its own `git` repository.

It has been tested (so far) on the following platform(s):

* Fedora 39 (using `g++ v13`).

See the **Building** section, below, for the location of that repository.

## Dependencies

this program uses the `gtkmm-4.0` package for its windowing framework, 

For the HTML-viewing widget in that framework, it uses the WebKit-GTK bindings.

For the markdown-to-HTML parser it uses the `cmark` package.

Instructions to install these packages are as simple as

``` console
$ sudo dnf install -y gtkmm4.0-devel webkitgtk6.0-devel cmark-devel
```

(For other flavours of Linux, the appropriate package management tool should
be used instead of `dnf`, *e.g.* `yum`, `apt`, `pkg` *etc.*).

## Building

Firstly, download a copy of the repo and navigate into its source root:

``` console
$ git clone https://github.com/tony-oliver/mdview && cd mdview
```

The top-level Makefile is used for co-ordinating the build process:

``` console
$ make [all]
```

Simple testing (displaying this file) can be run as a confidence test:

``` console
$ make run
```

Installation is accomplished with

``` console
$ make install
```

and uninstallation with

``` console
$ make uninstall
```

*(provided the project has not been cleaned; see below).*
 
Project cleanup can be performed with

``` console
$ make clean
```
