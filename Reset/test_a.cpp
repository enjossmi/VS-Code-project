#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "filesystem.hpp" 

TEST_CASE("File operations", "[File]") {
    File file("test.txt", "Hello, World!");

    SECTION("Read content") {
        REQUIRE(file.readContent() == "Hello, World!");
    }

    SECTION("Write content") {
        file.writeContent("New Content");
        REQUIRE(file.readContent() == "New Content");
        REQUIRE(file.size == 11);
    }

    SECTION("Append content") {
        file.appendContent(" More");
        REQUIRE(file.readContent() == "Hello, World! More");
        REQUIRE(file.size == 17);
    }

    SECTION("Delete content") {
        file.deleteContent();
        REQUIRE(file.readContent().empty());
        REQUIRE(file.size == 0);
    }
}

TEST_CASE("Directory operations", "[Directory]") {
    auto root = std::make_shared<Directory>("root");

    SECTION("Create and delete files") {
        auto file = root->createFile("file1.txt", "Content");
        REQUIRE(root->files.size() == 1);
        REQUIRE(root->files[0]->name == "file1.txt");

        root->deleteFile("file1.txt");
        REQUIRE(root->files.empty());
    }

    SECTION("Create and delete directories") {
        auto dir = root->createDirectory("subdir");
        REQUIRE(root->directories.size() == 1);
        REQUIRE(root->directories[0]->name == "subdir");

        root->deleteDirectory("subdir");
        REQUIRE(root->directories.empty());
    }

    SECTION("List contents") {
        root->createFile("file1.txt", "Content");
        root->createDirectory("subdir");
        auto contents = root->listContents();
        REQUIRE(contents.size() == 2);
        REQUIRE(std::find(contents.begin(), contents.end(), "file1.txt") != contents.end());
        REQUIRE(std::find(contents.begin(), contents.end(), "subdir/") != contents.end());
    }

    SECTION("Change directory") {
        auto subdir = root->createDirectory("subdir");
        auto current = root->getDirectory("subdir");
        REQUIRE(current == subdir);
    }
}

TEST_CASE("FileDescriptor operations", "[FileDescriptor]") {
    auto file = std::make_shared<File>("test.txt", "Hello, World!");
    FileDescriptor fd;

    SECTION("Open and read file") {
        fd.open(file);
        REQUIRE(fd.read(5) == "Hello");
        REQUIRE(fd.read(5) == ", Wor");
    }

    SECTION("Write to file") {
        fd.open(file);
        fd.write("New Content");
        REQUIRE(file->readContent() == "New Content");
    }

    SECTION("Close file") {
        fd.open(file);
        fd.close();
        REQUIRE(fd.read(5).empty());
    }
}

TEST_CASE("FileSystem operations", "[FileSystem]") {
    FileSystem fs;

    SECTION("Mount and unmount filesystem") {
        fs.mount();
        REQUIRE(fs.rootDirectory->name == "root");
        fs.unmount();
    }

    SECTION("Create and navigate directories") {
        fs.currentDirectory->createDirectory("dir1");
        REQUIRE(fs.currentDirectory->directories.size() == 1);

        auto dir1 = fs.currentDirectory->getDirectory("dir1");
        fs.currentDirectory = dir1;
        REQUIRE(fs.currentDirectory->name == "dir1");

        fs.currentDirectory = fs.currentDirectory->parentDirectory;
        REQUIRE(fs.currentDirectory->name == "root");
    }
}
