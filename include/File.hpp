#ifndef FILE_HPP
#define FILE_HPP

class TreeNode;
#include "HashMap.hpp"
#include <ctime>

class File{
private:
    std::string fileName;
    TreeNode* root;
    TreeNode* activeVersion;
    time_t modifiedTime;
    HashMap<int, TreeNode*> versionMap;
public:
    int totalVersions;
    File(std::string filename);
    ~File();
    TreeNode* getActiveVersion();
    void setActiveVersion(TreeNode* newActiveVersion);
    bool versionIdExist(int versionId);
    HashMap<int, TreeNode*>& getVersionMap();
    void setModifiedTime();
    time_t getModifiedTime();
    void deleteTree(TreeNode* node);
    const std::string getFileName() const;
};

#endif