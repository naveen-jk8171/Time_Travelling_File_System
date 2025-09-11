#include "File.hpp"
#include "TreeNode.hpp"

File::File(std::string fileName){
    root = new TreeNode(NULL, 0);
    versionMap.insertItem(0, root);
    modifiedTime = time(nullptr);
    totalVersions = 0;
    activeVersion = root;
    this->fileName = fileName;
}
void File::deleteTree(TreeNode* node) {
    for (auto c : node->getChildren()) deleteTree(c);
    delete node;
}
File::~File(){deleteTree(root);}
TreeNode* File::getActiveVersion(){
    return activeVersion;
}
void File::setActiveVersion(TreeNode* newActiveVersion){
    activeVersion = newActiveVersion;
}
bool File::versionIdExist(int versionId){
    return versionMap.contains(versionId);
}
HashMap<int, TreeNode*>& File::getVersionMap(){
    return versionMap;
}
void File::setModifiedTime(){
    modifiedTime = time(nullptr);
}
time_t File::getModifiedTime(){
    return modifiedTime;
}
const std::string File::getFileName() const{
    return fileName;
}