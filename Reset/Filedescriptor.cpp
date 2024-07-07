#include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : file(nullptr), position(0) {}

void FileDescriptor::open(const std::shared_ptr<File>& file) {
    this->file = file;
    position = 0;
}

void FileDescriptor::close() {
    file = nullptr;
    position = 0;
}

std::string FileDescriptor::read(int size) {
    if (file) {
        std::string content = file->content.substr(position, size);
        position += size;
        return content;
    }
    return "";
}

void FileDescriptor::write(const std::string& content) {
    if (file) {
        file->writeContent(content);
        position = content.size();
    }
}