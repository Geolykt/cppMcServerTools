/*
 ============================================================================
 Name        : LogSanitizer.cpp
 Author      : Geolykt
 Version     : 1.0.0
 Copyright   : Copyright © 2020 Geolykt <admin@geolykt.de>
 ---------------------------------------------------------------------
 DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

 DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

 0. You just DO WHAT THE FUCK YOU WANT TO.
 ---------------------------------------------------------------------
 Description : A sanitiser that removes IP addresses from various files.
               Meant to be used for minecraft server logs, but can be used
               for other usecases as well.
 ============================================================================
 */

#define BUILD_VERSION "1.0.0"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <getopt.h>

// Both of regexes were copied from https://stackoverflow.com/a/17871737
// I tried to create my own, but I wouldn't like for them to bork them for a production use application, so I copied them.
const std::regex ip4("((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])");
const std::regex ip6("(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))");

/**
 * @brief  Copies a file with a given path to a file whose path if the same, but the ".clean" extension is appended to it.
 * @brief  The resulting file has all ipv4 and ipv6 addresses stripped away or replaced.
 * @param  path The path to the file
 * @param  replacement The replacement that should be used in case an ip-address was found, empty to omit the line
 * @return 0 if the operation succeeded, non-0 if an error occurred.
 */
int handleFile(std::string path, std::string replacement) {
    std::ifstream fileInput(path);
    std::fstream fileOutput;
    fileOutput.open(path + ".clean", std::ios::out);
    if (!fileInput) {
        std::cout << "The requested file (" << path << ") does not exist!"
                << std::endl;
        return 1;
    } else {
        std::string line;
        while (std::getline(fileInput, line)) {
            if (replacement.empty()) {
                if (!(std::regex_search(line, ip4) || std::regex_search(line, ip6))) {
                    // Does not contain an IP -> post line, otherwise omit
                    fileOutput << line << std::endl;
                }
            } else {
                // Replace
                line = std::regex_replace(line, ip4, replacement);
                line = std::regex_replace(line, ip6, replacement);
                fileOutput << line << std::endl;
            }
        }
        return 0;
    }
}

/**
 * @brief  Prints the help information to standard out.
 * @return Always 0
 */
int printHelp() {
    printf("Usage: LogSanitizer [-c|-C] [-d|-D] [-o] [-v] [-q] [-f <file>] [-r <replacement>]\n");
    printf("Arguments:\n");
    printf("    c: Compresses resulting files and removes the uncompressed resulting file\n");
    printf("    C: Compresses resulting files and keeps the uncompressed resulting file\n");
    printf("    d: Decompresses all archives and keeps the source file (recursive!)\n");
    printf("    D: Decompresses all archives and removes the source file (recursive!)\n");
    printf("    o: If set (de-)compression will not prompt when overwriting existing files.\n");
    printf("    v: If set (de-)compression will have the verbose flag.\n");
    printf("    q: If set quite mode is active and the replace information is not shown.\n");
    printf("    f: The file/folder that should be targeted.\n");
    printf("    r: The replacement used when an ip-address was found. If not set the line is omitted.\n");
    std::cout << "*** LogSanitizer v" << BUILD_VERSION << " by Geolykt <admin@geolykt.de> ***" << std::endl;
    std::cout << "Binary built on " << __DATE__ << " " << __TIME__ << std::endl;
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return printHelp();
    }
    int decompression = 0;
    int compression = 0;
    int overwrite = 0;
    int verbose = 0;
    int quiet = 0;
    std::string path = "latest.log";
    std::string replacement = "";

    int c;
    while ((c = getopt (argc, argv, "cCdDovqf:r:h?")) != -1) {
        switch (c) {
        case 'c':
            compression = 1;
            continue;
        case 'C':
            compression = 2;
            continue;
        case 'd':
            decompression = 1;
            continue;
        case 'D':
            decompression = 2;
            continue;
        case 'f':
            path = optarg;
            continue;
        case 'o':
            overwrite = 1;
            continue;
        case 'v':
            verbose = 1;
            continue;
        case 'q':
            quiet = 1;
            continue;
        case 'r':
            replacement = optarg;
            continue;
        case '?':
        case 'h':
        default:
            return printHelp();
        }
    }

    std::cout << "Handling " << path << " ..." << std::endl;
    std::filesystem::path parentFile = std::filesystem::path(path); // @suppress("Symbol is not resolved")
    if (!std::filesystem::exists(parentFile)) {
        std::cout << "File/Folder does not exist! (aborting)" << std::endl;
        return 1;
    }
    if (!std::filesystem::is_directory(parentFile)) {
        return handleFile(path, replacement);
    }

    if (decompression != 0) {
        std::string cmd = std::string("gzip -dr");
        if (decompression == 1) {
            cmd.append("k");
        }
        if (verbose) {
            cmd.append("v");
        }
        if (overwrite) {
            cmd.append("f ");
        } else {
            cmd.append(" ");
        }
        system(cmd.append(path).c_str());
    }

    std::string compressionCommand = "";
    if (compression == 1) {
        compressionCommand = "zstd --ultra -22 --rm ";
    } else if (compression == 2) {
        compressionCommand = "zstd --ultra -22 -k ";
    }
    if (verbose) {
        compressionCommand.append("-v ");
    } else {
        compressionCommand.append("-q ");
    }
    if (overwrite) {
        compressionCommand.append("-f ");
    }

    for (const auto &p : std::filesystem::recursive_directory_iterator(
            parentFile)) {
        // Avoid directories, .gz, .zst and .clean files
        if (!(p.is_directory()
                || p.path().string().ends_with(".gz")
                || p.path().string().ends_with(".clean")
                || p.path().string().ends_with(".zst"))) {
            if (!quiet) {
                std::cout << "Working on " << p.path().string() << std::endl;
            }
            if (handleFile(p.path().string(), replacement)) {
                return 1;
            }
            if (compression != 0) {
                system(std::string(compressionCommand).append(p.path().string().append(".clean")).c_str());
            }
        }
    }
    return 0;
}

