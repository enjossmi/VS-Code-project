#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP

#include <string>
#include <memory>
#include "File.hpp" // Include necessary headers

class FileDescriptor {
public:
    std::shared_ptr<File> file;
    int position;

    FileDescriptor();
    void open(const std::shared_ptr<File>& file);
    void close();
    std::string read(int size);
    void write(const std::string& content);
};

#endif // FILEDESCRIPTOR_HPP