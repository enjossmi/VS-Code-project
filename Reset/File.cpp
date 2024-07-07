// File.cpp
#include "File.hpp"

File::File(const std::string& name, const std::string& content)
    : name(name), content(content), size(content.size()) {}

std::string File::readContent() const {
    return content;
}

void File::writeContent(const std::string& newContent) {
    content = newContent;
    size = content.size();
}

void File::appendContent(const std::string& newContent) {
    content += newContent;
    size = content.size();
}

void File::deleteContent() {
    content = "";
    size = 0;
}


