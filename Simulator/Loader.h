#pragma once

#include "ArgsParser.h"
#include <dirent.h>


class Loader {
    public:
        /**
         * @file Loader.h
         * @brief Declares functions for loading and executing shared object (.so) files.
         */

        /**
         * @brief Checks if a filename has a .so extension.
         * @param filename The filename to check.
         * @return Non-zero if the extension is .so, zero otherwise.
         */
        static int HasSOExtension(const char *filename);

        /**
         * @brief Loads shared objects from the directory specified in ParsedArgs and executes their run function.
         * @param args ParsedArgs structure containing mode and relevant directories.
         * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
         */
        static int LoadSharedObjects(const ParsedArgs& args);

        /**
         * @brief Loads and executes all .so files in the given directory.
         * @param dir Pointer to opened DIR structure.
         * @param so_dir Directory path as a string.
         * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
         */
        static int LoadSOFilesFromDir(DIR* dir, const std::string& so_dir);

        static int LoadSharedFile(const std::string& so_file);

        static std::string filenameFromPath(const std::string& fullpath);
};



