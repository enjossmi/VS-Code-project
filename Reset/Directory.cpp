// Directory.cpp
#include "Directory.hpp"
#include "File.hpp" 

Directory::Directory(const std::string& name, const std::shared_ptr<Directory>& parent)
    : name(name), parentDirectory(parent) {}

std::shared_ptr<File> Directory::createFile(const std::string& fileName, const std::string& content) {
    auto newFile = std::make_shared<File>(fileName, content);
    files.push_back(newFile);
    return newFile;
}
/// @brief 
/// @param fileName 
void Directory::deleteFile(const std::string& fileName) {
    files.erase(std::remove_if(files.begin(), files.end(),
        [&fileName](const std::shared_ptr<File>& file) { return file->name == fileName; }), files.end());
}

std::shared_ptr<Directory> Directory::createDirectory(const std::string& dirName) {
    auto newDirectory = std::make_shared<Directory>(dirName, shared_from_this());
    directories.push_back(newDirectory);
    return newDirectory;
}

void Directory::deleteDirectory(const std::string& dirName) {
    directories.erase(std::remove_if(directories.begin(), directories.end(),
        [&dirName](const std::shared_ptr<Directory>& dir) { return dir->name == dirName; }), directories.end());
}

std::shared_ptr<Directory> Directory::getDirectory(const std::string& dirName) {
    auto it = std::find_if(directories.begin(), directories.end(),
        [&dirName](const std::shared_ptr<Directory>& dir) { return dir->name == dirName; });
    if (it != directories.end()) {
        return *it;
    }
    return nullptr;
}

std::vector<std::string> Directory::listContents() const {
    std::vector<std::string> contents;
    for (const auto& file : files) {
        contents.push_back(file->name);
    }
    for (const auto& dir : directories) {
        contents.push_back(dir->name + "/");
    }
    return contents;
}
