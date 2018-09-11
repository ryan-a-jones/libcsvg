[![Build Status](https://travis-ci.org/ryaxx/libcsvg.svg?branch=master)](https://travis-ci.org/ryaxx/libcsvg)

Lib CSVG
====

_SVG Parser, Manipulator, and Writer written in `C`_

## Why Another SVG Parser?

This library was written in response to frustrations
with other SVG Libraries (namely `librsvg`).

The main goals of this library are to provide:

* A clean and easy to use interface for parsing and
    constructing SVG trees that is minimally prohibitive
    for accessing element data.

* The ability to flatten, transform, or otherwise
    manipulate SVG Elements while retaining attributes
    and namespace information.

* A library with minimum dependencies. `librsvg`, for example,
    has dependencies to `cairo` which has many dependencies
    including `X11` utilities which can be prohibitive to many
    systems (server-side, embedded, etc.).

## Status

LibCSVG is currently in alpha and is nowhere near full functionality.
If you would like to help out, see `CONTRIBUTING.md`.

## Installing
Dependencies are:

* Autotools (since there is not a LibCSVG release yet)
* libxml2
* cmocka (only required to run tests)

On Debian/Ubuntu you should be able to satisfy these dependencies
by running

```
sudo apt-get install autotools-dev libxml2-dev libcmocka-dev
```

To install, run:

```
autoreconf -i
./configure
make
sudo make install
```

To run tests (after configuring):

```
make check
```
