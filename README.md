# **EUT** - Ethan's Utils

EUT is a small collection of (somewhat) useful toolchain and utility programs for automating some repetetive tasks in programming/UNIX computer usage.
Some programs are useful for quick things (getting a random number from the best entropy source), whereas others are good for experiments (mem-destroyer).

A quick description of each program is provided in its corresponding **README.md** file in the respective directory. Unless otherwise specified in each subdirectory, software is licensed under the ISC software licence.

## Why?

Because I have got a whole load of little testing programs stored up around the system which I occasionally use, but which I never shared before. I thought I might as well do a public service and share some of the more useful ones with the world.

Also, occasionally it is good to do "programming workouts" - small little pieces of problem solving or engineering which keep your mind sharp and in the right frame of mind to write software. If you don't use it, you loose it. The successful results of these occasional experiments are also dumped here. These will also be periodically updated for the same reason.

## How?

Tools must be built individually. To build a single tool, run ``make`` or ``go build .`` in a single subdirectory.

Most tools are written in mostly-POSIX C99. Some are written in pure Go. Makefiles should be POSIX compliant. To download golang, go to <golang.org>
