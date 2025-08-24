#include "ArgsParser.h"
#include "Loader.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <dirent.h> // For directory operations
#include <dlfcn.h>  // For dynamic loading of shared 
#include <iostream> // For input/output operations
#include <cstring>
#include "AlgorithmRegistrar.h"
#include "GameManagerRegistrar.h"

int Loader::HasSOExtension(const char *filename) {
    // This function checks if a filename has a .so extension.
    const char *ext = strrchr(filename, '.'); // Find the last occurrence of '.'
    return ext && strcmp(ext, ".so") == 0; // Check if the extension is .so
}

int Loader::LoadSharedObjects(const ParsedArgs& args) {
    // This function loads shared objects based on the parsed arguments and returns an exit code.
    // Determine the directory to load shared objects from based on the mode.
    std::string so_dir;
    if (args.mode == ParsedArgs::Mode::Comparative)
        so_dir = args.game_managers_folder;
    else if (args.mode == ParsedArgs::Mode::Competition)
        so_dir = args.algorithms_folder;
    else {
        std::cerr << "Unsupported mode for loading shared objects." << std::endl;
        return EXIT_FAILURE;
    }
    DIR *dir = opendir(so_dir.c_str());
    if (!dir) {
        std::cerr << "Failed to open directory: " << so_dir << std::endl;
        return EXIT_FAILURE;
    }
    int result = LoadSOFilesFromDir(dir, so_dir);
    closedir(dir);

    int file_result = 0;
    if (args.mode == ParsedArgs::Mode::Comparative) {
        int r1 = LoadSharedFile(args.algorithm1);
        if (args.algorithm1 == args.algorithm2) {
            AlgorithmRegistrar::getAlgorithmRegistrar().copyLastEntry();

        } else {
            int r2 = LoadSharedFile(args.algorithm2);
            if (r1 != 0 || r2 != 0)
                file_result = -1;
        }
    }
    else if (args.mode == ParsedArgs::Mode::Competition) {
        int r = LoadSharedFile(args.game_manager_so);
        if (r != 0) file_result = -1;
    }

    return (result != 0 || file_result != 0) ? -1 : 0;
}

std::string Loader::filenameFromPath(const std::string& fullpath)
{
    namespace fs = std::filesystem;
    fs::path p(fullpath);

    // If path ends with a separator (no filename), use the last non-empty component.
    if (!p.has_filename()) {
        p = p.parent_path();
    }
    return p.filename().string();  // e.g., "/usr/local/file.so" -> "file.so"
}

int Loader::LoadSharedFile(const std::string& so_file_path) {
    try {
        std::string filename_only = filenameFromPath(so_file_path);
        // Register Algorithm or GameManager factory with filename
        if (std::equal(filename_only.begin(), filename_only.begin()+9, "Algorithm")) {
            AlgorithmRegistrar::getAlgorithmRegistrar().createAlgorithmFactoryEntry(filename_only);
        } else if (std::equal(filename_only.begin(), filename_only.begin()+11, "GameManager")) {
            void *handle = dlopen(so_file_path.c_str(), RTLD_NOW);
            if (!handle) {
                std::cerr << "dlopen failed for " << so_file_path << ": " << dlerror() << std::endl;
                return -1;
            }
            // After dlopen, the .so should have registered a factory. Now update the filename for the last one.
            auto& registrar = GameManagerRegistrar::getGameManagerRegistrar();
            registrar.setLastFilename(filename_only);
            return 0;
        } else {
            void *handle = dlopen(so_file_path.c_str(), RTLD_NOW);
            if (!handle) {
                std::cerr << "dlopen failed for " << so_file_path << ": " << dlerror() << std::endl;
                return -1;
            }
            return 0;
        }
        void *handle = dlopen(so_file_path.c_str(), RTLD_NOW);  // Load the shared object file
        if (!handle) {
            std::cerr << "dlopen failed for " << so_file_path << ": " << dlerror() << std::endl;
            AlgorithmRegistrar::getAlgorithmRegistrar().removeLast();
            return -1; // Return error code instead of exiting
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in LoadSharedFile: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception caught in LoadSharedFile." << std::endl;
        return -1;
    }
}

int Loader::LoadSOFilesFromDir(DIR* dir, const std::string& so_dir) {
    // This function iterates through the directory and loads each .so file.
    struct dirent *entry;
    int result = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG && entry->d_type != DT_LNK)
            continue; // Skip non-regular files
        if (!HasSOExtension(entry->d_name)) // Skip files without .so extension
            continue;
        const std::string& so_path = so_dir + "/" + entry->d_name; // Construct the full path to the shared object file
        int load_result = LoadSharedFile(so_path);
        if (load_result != 0) {
            result = load_result; // Mark error but continue loading others
        }
    }
    return result;
}
