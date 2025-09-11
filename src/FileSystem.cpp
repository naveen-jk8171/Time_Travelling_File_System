#include "FileSystem.hpp"
#include "File.hpp"
#include "TreeNode.hpp"
#include <iostream>

void FileSystem::createFile(std::string &filename){
    if (files.searchTable(filename)){
        std::cout << filename << " already exist..." << std::endl;
        return;
    }
    File* newfile = new File(filename);
    files.insertItem(filename, newfile);
    recentFile.addFile(newfile, newfile->getModifiedTime());
    mostEditedFile.addFile(newfile, newfile->totalVersions+1);
    newfile->getActiveVersion()->setMessage("root");
    newfile->getActiveVersion()->setSnapshotTimestamp();
    std::cout << filename << " created..." << std::endl;
}
void FileSystem::readFile(std::string &filename){
    File* file = files.searchTable(filename);
    if (!file){
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    TreeNode* activeVersion = file->getActiveVersion();
    std::cout << "File: " << filename << ", Version: " << activeVersion->getVersionId() << std::endl;
    std::cout << "Content: " << activeVersion->getContent() << std::endl;
}
void FileSystem::insertInFile(std::string &filename, std::string &content){
    File* file = files.searchTable(filename);
    if (!file){
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    TreeNode* activeVersion = file->getActiveVersion();
    file->setModifiedTime();
    if (activeVersion->getMessage() != "" || activeVersion->getSnapshotTimestamp() != 0){
        TreeNode* newVersion = new TreeNode(activeVersion, file->totalVersions+1);
        file->totalVersions++;
        int newVersionId = newVersion->getVersionId();
        newVersion->insertContent(activeVersion->getContent());
        activeVersion->addChildren(newVersion);
        newVersion->insertContent(content);
        file->setActiveVersion(newVersion);
        file->getVersionMap().insertItem(newVersionId, newVersion);
        int fileI = mostEditedFile.findInd(file);
        mostEditedFile.update(fileI, file->totalVersions+1);
        mostEditedFile.tickleUp(fileI);
        std::cout << "New version: " << newVersion->getVersionId() << " created..." << std::endl;
    }else{
        activeVersion->insertContent(content);
        activeVersion->setModifiedTimestamp();
    }
    int fileInd = recentFile.findInd(file);
    recentFile.tickleUp(fileInd);
    std::cout << "File: " << filename << ", Version: " << file->getActiveVersion()->getVersionId() << std::endl;
    std::cout << "Insertion done..." << std::endl;
}
void FileSystem::updateInFile(std::string &filename, std::string &content){
    File* file = files.searchTable(filename);
    if (!file){
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    TreeNode* activeVersion = file->getActiveVersion();
    file->setModifiedTime();
    if (activeVersion->getMessage() != "" || activeVersion->getSnapshotTimestamp() != 0){
        TreeNode* newVersion = new TreeNode(activeVersion, file->totalVersions+1);
        file->totalVersions++;
        int newVersionId = newVersion->getVersionId();
        activeVersion->addChildren(newVersion);
        newVersion->updateContent(content);
        file->setActiveVersion(newVersion);
        file->getVersionMap().insertItem(newVersionId, newVersion);
        int fileI = mostEditedFile.findInd(file);
        mostEditedFile.update(fileI, file->totalVersions+1);
        mostEditedFile.tickleUp(fileI);
        std::cout << "New version: " << newVersion->getVersionId() << " created..." << std::endl;
    }else{
        activeVersion->updateContent(content);
        activeVersion->setModifiedTimestamp();
    }
    int fileInd = recentFile.findInd(file);
    recentFile.tickleUp(fileInd);
    std::cout << "File: " << filename << ", Version: " << file->getActiveVersion()->getVersionId() << std::endl;
    std::cout << "Updated..." << std::endl;
}
void FileSystem::snapshotFile(std::string &filename, std::string &message){
    File* file = files.searchTable(filename);
    if (!file){
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    TreeNode* activeVersion = file->getActiveVersion();
    if (activeVersion->getMessage() == "" && activeVersion->getSnapshotTimestamp() == 0){
        activeVersion->setMessage(message);
        activeVersion->setSnapshotTimestamp();
        activeVersion->setModifiedTimestamp();
        std::cout << "File: " << filename << ", Version: " << file->getActiveVersion()->getVersionId() << std::endl;
        std::cout << "Snapshot taken..." << std::endl;
    }else{
        std::cout << "File: " << filename << ", Version: " << file->getActiveVersion()->getVersionId() << std::endl;
        std::cout << "Snapshot already exists for this version..." << std::endl;
    }
}
void FileSystem::rollbackFile(std::string &filename, int versionId){
    File* file = files.searchTable(filename);
    if (!file){
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    if (file->versionIdExist(versionId)){
        file->setActiveVersion(file->getVersionMap().searchTable(versionId));
        std::cout << filename << " rolled back to version " << versionId << "..." << std::endl;
    }else{
        if (versionId != INT_MAX){
            std::cout << "Invalid version ID..." << std::endl;
            return;
        }
        TreeNode* parent = file->getActiveVersion()->getParent();
        if (parent){
            file->setActiveVersion(parent);
            std::cout << filename << " rolled back to version " << parent->getVersionId() << "..." << std::endl;
        }else{
            std::cout << "No previous versions found..." << std::endl;
        }
    }
}
void FileSystem::historyOfFile(std::string &filename){
    if (!files.contains(filename)) {
        std::cout << filename << " not found..." << std::endl;
        return;
    }
    File* file = files.searchTable(filename);
    TreeNode* temp = file->getActiveVersion();
    std::vector<TreeNode*> vec;
    if (temp->getSnapshotTimestamp() == 0) temp = temp->getParent();
    while(temp){
        vec.push_back(temp);
        temp = temp->getParent();
    }
    std::cout << filename << " history: " << std::endl;
    for (int i = vec.size()-1; i >= 0; i--){
        temp = vec[i];
        std::cout << "Version: " << temp->getVersionId() << ", Snapshot Message: " << temp->getMessage() << ", Snapshot Timestamp: " << ctime(&temp->getSnapshotTimestamp());
    }
}
void FileSystem::recentFiles(int num){
    if (recentFile.getHeapSize()==0){
        std::cout << "No recent files were found..." << std::endl;
        return;
    }
    if (num > recentFile.getHeapSize()){
        std::cout << "Requested " << num << " files but only " << recentFile.getHeapSize() << " recent files are there..." << std::endl;
        return;
    }
    std::vector<std::pair<File*, time_t>> vec;
    std::cout << num << " recent files: " << std::endl;
    while(num > 0){
        vec.push_back(recentFile.popFile());
        num--;
    }
    for (auto i : vec){
        recentFile.addFile(i.first, i.second);
        std::cout << "File: " << i.first->getFileName() << ", Modification Time: " << ctime(&i.second);
    }
}
void FileSystem::biggestTrees(int num){
    if (mostEditedFile.getHeapSize() == 0) {
        std::cout << "No files were found..." << std::endl;
        return;
    }
    if (num > mostEditedFile.getHeapSize()) {
        std::cout << "Requested " << num << " files but only " << mostEditedFile.getHeapSize() << " files are there..." << std::endl;
        return;
    }
    std::vector<std::pair<File*, int>> vec;
    std::cout << num << " most edited files: " << std::endl;
    while(num > 0){
        vec.push_back(mostEditedFile.popFile());
        num--;
    }
    for (auto i : vec){
        mostEditedFile.addFile(i.first, i.second);
        std::cout << "File: " << i.first->getFileName() << ", Versions: " << i.second << std::endl;
    }
}