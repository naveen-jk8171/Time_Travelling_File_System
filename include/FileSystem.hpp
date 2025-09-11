#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include "HashMap.hpp"
#include "Heap.hpp"
class File;

class FileSystem{
private:
    HashMap<std::string, File*> files;
    Heap<time_t> recentFile = Heap<time_t>();
    Heap<int> mostEditedFile = Heap<int>();

public:
    void createFile(std::string &filename);
    void readFile(std::string &filename);
    void insertInFile(std::string &filename, std::string &content);
    void updateInFile(std::string &filename, std::string &content);
    void snapshotFile(std::string &filename, std::string &message);
    void rollbackFile(std::string &filename, int versionId);
    void historyOfFile(std::string &filename);
    void recentFiles(int num);
    void biggestTrees(int num);
};

#endif