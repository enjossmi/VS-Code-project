#include <iostream>
#include <algorithm> 
#include <string>
#include <memory>
#include "FileSystem.hpp"
#include "FileDescriptor.hpp"
#include "Directory.hpp"
#include "File.hpp"

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
