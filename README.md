# **EUT** - Ethan's Utils

EUT is a small collection of (somewhat) useful toolchain and utility programs for automating some repetetive tasks in programming/UNIX computer usage.
Some programs are useful for quick things (getting a random number from the best entropy source), whereas others are good for experiments (mem-destroyer).

A quick description of each program is provided in its corresponding **README.md** file in the respective directory.

## Why?

Because I have got a whole load of little testing programs stored up around the system which I occasionally use, but which I never shared before. I thought i might aswell do a public service and share some of the more useful ones with the world.

## How?

Run ``make`` in the root of the project and all subdirectories will be built. The executables will remain in their directories. Automatic compilation of Go tools is not yet supported. To build a Go tool, see the below instructions.

To build a single tool, run ``make`` or ``go build .`` in a single subdirectory.

Most tools are written in mostly-POSIX C99. Some are written in pure Go. It is likely that you require GNU Make to use the included Makefiles (GNU Make syntax/functions used in many places). To download golang, go to <golang.org>
