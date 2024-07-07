// File.hpp
#ifndef FILE_HPP
#define FILE_HPP

#include <string>

class File {
public:
    std::string name;
    int size;
    std::string content;

    File(const std::string& name, const std::string& content);
    std::string readContent() const;
    void writeContent(const std::string& newContent);
    void appendContent(const std::string& newContent);
    void deleteContent();
};

#endif // FILE_HPP
