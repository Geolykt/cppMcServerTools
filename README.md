# cppMcServerTools
A  collection of standalones written in C++ to manage your Minecraft server

## Compiling
Every application is a standalone and should, if possible not contain multiple source files or libraries.
However the applications should be compiled with g++ and the c++20 standard, as that is what they are tested on.
We recommend to use a Linux based operating system as the applications are intended to be used on servers,
however It's likely that non-linux operating systems could compile and run the applications just as well.

## Contributing
Everyone is free to add pretty much everything to this repository that is Minecraft related and written in C++.
The licensing should be as free (BSD, WTFPL and similar) as possible and not be oversized licenses like the GPL

## Licensing
As of yet every application is licensed differently, the exact license is shown in the header of the application,
please note that it's likely that I'm going to unify this license, but in the meantime there isn't a common license.

## Index
 * [LogSanitizer](#LogSanitizer) - An application that strips ip-addresses away, meant for server logs.

### LogSanitizer
LogSanitizer's subcommands can be found by invoking the application directly via CLI.
Flags and arguemnts of the application are POSIX-style
It works purely in a CLI fashion and supports automatic decompression, compression, replacing as well as recusive director sanitisation.
 * Requires GZIP in order to decompress stuff
 * Requires ZSTD in order to compress stuff
 * Requires C++20 in order to compile
 * Requires getopt in order to parse arguments and flags (should be installed on most Linux systems)
