// Directory.hpp
#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "File.hpp" // Include File.hpp to use File class

class File; // Forward declaration

class Directory: public std::enable_shared_from_this<Directory> {
public:
    std::string name;
    std::vector<std::shared_ptr<File>> files;
    std::vector<std::shared_ptr<Directory>> directories;
    std::shared_ptr<Directory> parentDirectory;

    Directory(const std::string& name, const std::shared_ptr<Directory>& parent = nullptr);
    std::shared_ptr<File> createFile(const std::string& fileName, const std::string& content);
    void deleteFile(const std::string& fileName);
    std::shared_ptr<Directory> createDirectory(const std::string& dirName);
    void deleteDirectory(const std::string& dirName);
    std::shared_ptr<Directory> getDirectory(const std::string& dirName);
    std::vector<std::string> listContents() const;
};

#endif // DIRECTORY_HPP