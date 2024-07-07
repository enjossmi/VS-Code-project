// FileSystem.hpp
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <iostream>
#include <memory>
#include <string>
#include "Directory.hpp"
#include "FileDescriptor.hpp"

class FileSystem {
public:
    std::shared_ptr<Directory> rootDirectory;
    std::shared_ptr<Directory> currentDirectory;

    FileSystem();
    void mount();
    void unmount();
};

#endif // FILESYSTEM_HPP