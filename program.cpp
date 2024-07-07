#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class File {
public:
    std::string name;
    int size;
    std::string content;

    File(const std::string& name, const std::string& content)
        : name(name), content(content), size(content.size()) {}

    std::string readContent() const {
        return content;
    }

    void writeContent(const std::string& newContent) {
        content = newContent;
        size = content.size();
    }

    void appendContent(const std::string& newContent) {
        content += newContent;
        size = content.size();
    }

    void deleteContent() {
        content = "";
        size = 0;
    }
};

class Directory : public std::enable_shared_from_this<Directory> {
public:
    std::string name;
    std::vector<std::shared_ptr<File>> files;
    std::vector<std::shared_ptr<Directory>> directories;
    std::shared_ptr<Directory> parentDirectory;

    Directory(const std::string& name, const std::shared_ptr<Directory>& parent = nullptr) 
        : name(name), parentDirectory(parent) {}

    std::shared_ptr<File> createFile(const std::string& fileName, const std::string& content) {
        auto newFile = std::make_shared<File>(fileName, content);
        files.push_back(newFile);
        return newFile;
    }

    void deleteFile(const std::string& fileName) {
        files.erase(std::remove_if(files.begin(), files.end(),
            [&fileName](const std::shared_ptr<File>& file) { return file->name == fileName; }), files.end());
    }

    std::shared_ptr<Directory> createDirectory(const std::string& dirName) {
        auto newDirectory = std::make_shared<Directory>(dirName, shared_from_this());
        directories.push_back(newDirectory);
        return newDirectory;
    }

    void deleteDirectory(const std::string& dirName) {
        directories.erase(std::remove_if(directories.begin(), directories.end(),
            [&dirName](const std::shared_ptr<Directory>& dir) { return dir->name == dirName; }), directories.end());
    }

    std::shared_ptr<Directory> getDirectory(const std::string& dirName) {
        auto it = std::find_if(directories.begin(), directories.end(),
            [&dirName](const std::shared_ptr<Directory>& dir) { return dir->name == dirName; });
        if (it != directories.end()) {
            return *it;
        }
        return nullptr;
    }

    std::vector<std::string> listContents() const {
        std::vector<std::string> contents;
        for (const auto& file : files) {
            contents.push_back(file->name);
        }
        for (const auto& dir : directories) {
            contents.push_back(dir->name + "/");
        }
        return contents;
    }
};

class FileDescriptor {
public:
    std::shared_ptr<File> file;
    int position;

    FileDescriptor() : file(nullptr), position(0) {}

    void open(const std::shared_ptr<File>& file) {
        this->file = file;
        position = 0;
    }

    void close() {
        file = nullptr;
        position = 0;
    }

    std::string read(int size) {
        if (file) {
            std::string content = file->content.substr(position, size);
            position += size;
            return content;
        }
        return "";
    }

    void write(const std::string& content) {
        if (file) {
            file->writeContent(content);
            position = content.size();
        }
    }
};

class FileSystem {
public:
    std::shared_ptr<Directory> rootDirectory;
    std::shared_ptr<Directory> currentDirectory;

    FileSystem() {
        rootDirectory = std::make_shared<Directory>("root");
        currentDirectory = rootDirectory;
    }

    void mount() {
        std::cout << "File system mounted." << std::endl;
    }

    void unmount() {
        std::cout << "File system unmounted." << std::endl;
    }
};

int main() {
    FileSystem fs;
    fs.mount();

    std::string command;
    std::string fileName;
    std::string content;
    std::string directoryName;

    while (true) {
        std::cout << "Enter a command (create_file, create_directory, list_contents, read_file, write_file, append_file, delete_file, delete_directory, change_directory, exit): ";
        std::cin >> command;

        if (command == "create_file") {
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            std::cin.ignore(); // ignore newline
            std::cout << "Enter file content: ";
            std::getline(std::cin, content);
            fs.currentDirectory->createFile(fileName, content);
            std::cout << "File created." << std::endl;

        } else if (command == "create_directory") {
            std::cout << "Enter directory name: ";
            std::cin >> directoryName;
            fs.currentDirectory->createDirectory(directoryName);
            std::cout << "Directory created." << std::endl;

        } else if (command == "list_contents") {
            auto contents = fs.currentDirectory->listContents();
            std::cout << "Directory contents:" << std::endl;
            for (const auto& name : contents) {
                std::cout << name << std::endl;
            }

        } else if (command == "read_file") {
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            auto file = std::find_if(fs.currentDirectory->files.begin(), fs.currentDirectory->files.end(),
                                     [&fileName](const std::shared_ptr<File>& file) { return file->name == fileName; });
            if (file != fs.currentDirectory->files.end()) {
                FileDescriptor fd;
                fd.open(*file);
                std::cout << "File content: " << fd.read((*file)->size) << std::endl;
            } else {
                std::cout << "File not found." << std::endl;
            }

        } else if (command == "write_file") {
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            auto file = std::find_if(fs.currentDirectory->files.begin(), fs.currentDirectory->files.end(),
                                     [&fileName](const std::shared_ptr<File>& file) { return file->name == fileName; });
            if (file != fs.currentDirectory->files.end()) {
                std::cin.ignore(); // ignore newline
                std::cout << "Enter new content: ";
                std::getline(std::cin, content);
                (*file)->writeContent(content);
                std::cout << "File content updated." << std::endl;
            } else {
                std::cout << "File not found." << std::endl;
            }

        } else if (command == "append_file") {
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            auto file = std::find_if(fs.currentDirectory->files.begin(), fs.currentDirectory->files.end(),
                                     [&fileName](const std::shared_ptr<File>& file) { return file->name == fileName; });
            if (file != fs.currentDirectory->files.end()) {
                std::cin.ignore(); // ignore newline
                std::cout << "Enter content to append: ";
                std::getline(std::cin, content);
                (*file)->appendContent(content);
                std::cout << "File content appended." << std::endl;
            } else {
                std::cout << "File not found." << std::endl;
            }

        } else if (command == "delete_file") {
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            fs.currentDirectory->deleteFile(fileName);
            std::cout << "File deleted." << std::endl;

        } else if (command == "delete_directory") {
            std::cout << "Enter directory name: ";
            std::cin >> directoryName;
            fs.currentDirectory->deleteDirectory(directoryName);
            std::cout << "Directory deleted." << std::endl;

        } else if (command == "change_directory") {
            std::cout << "Enter directory name (.. to go up): ";
            std::cin >> directoryName;
            if (directoryName == "..") {
                if (fs.currentDirectory != fs.rootDirectory) {
                    fs.currentDirectory = fs.currentDirectory->parentDirectory; // Update currentDirectory
                    std::cout << "Changed directory to parent directory." << std::endl;
                } else {
                    std::cout << "Already in the root directory." << std::endl;
                }
            } else {
                auto newDir = fs.currentDirectory->getDirectory(directoryName);
                if (newDir) {
                    fs.currentDirectory = newDir; // Update currentDirectory
                    std::cout << "Changed directory to " << directoryName << std::endl;
                } else {
                    std::cout << "Directory not found." << std::endl;
                }
            }

        } else if (command == "exit") {
            break;

        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    fs.unmount();
    return 0;
}