
#include "FileSystem.hpp"



FileSystem::FileSystem() {
    rootDirectory = std::make_shared<Directory>("root");
    currentDirectory = rootDirectory;
}

void FileSystem::mount() {
    std::cout << "File system mounted." << std::endl;
}

void FileSystem::unmount() {
    std::cout << "File system unmounted." << std::endl;
}