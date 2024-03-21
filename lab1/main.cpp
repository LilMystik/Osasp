#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>

bool is_directory(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISDIR(statbuf.st_mode);
}

bool is_file(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISREG(statbuf.st_mode);
}

bool is_symbolic_link(const std::string& path) {
    struct stat statbuf;
    if (lstat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISLNK(statbuf.st_mode);
}

void print_entry(const std::string& path) {
    std::cout << path << std::endl;
}

void dirwalk(const std::string& directory, const std::vector<char>& options) {
    DIR* dir = opendir(directory.c_str());
    if (dir == nullptr) {
        std::cerr << "Failed to open directory: " << directory << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        std::string path = directory + "/" + name;

        if (name == "." || name == "..")
            continue;

        if (is_directory(path) && std::find(options.begin(), options.end(), 'd') != options.end()) {
            print_entry(path);
        } else if (is_file(path) && std::find(options.begin(), options.end(), 'f') != options.end()) {
            print_entry(path);
        } else if (is_symbolic_link(path) && std::find(options.begin(), options.end(), 'l') != options.end()) {
            print_entry(path);
        }

        if (is_directory(path) && !is_symbolic_link(path)) {
            dirwalk(path, options);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    std::string directory = "./";
    std::vector<char> options;

    int option;
    while ((option = getopt(argc, argv, "ldfs")) != -1) {
        switch (option) {
            case 'l':
            case 'd':
            case 'f':
            case 's':
                options.push_back(option);
                break;
            default:
                std::cerr << "Unknown option: " << static_cast<char>(option) << std::endl;
                return 1;
        }
    }

    if (optind < argc) {
        directory = argv[optind];
    }

    dirwalk(directory, options);

    return 0;
}