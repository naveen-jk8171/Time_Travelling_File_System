#include "TreeNode.hpp"

TreeNode::TreeNode(TreeNode* parent, int versionId){
    this->parent = parent;
    this->versionId = versionId;
    createdTimestamp = time(nullptr);
    snapshotTimestamp = 0;
    modifiedTimestamp = time(nullptr);
}
TreeNode::~TreeNode(){}
std::string TreeNode::getContent(){
    return content;
}
std::string TreeNode::getMessage(){
    return message;
}
TreeNode* TreeNode::getParent(){
    return parent;
}
int TreeNode::getVersionId(){
    return versionId;
}
void TreeNode::insertContent(std::string content){
    if (this->content.empty()) this->content = content;
    else this->content += " " + content;
}
void TreeNode::updateContent(std::string newContent){
    this->content = newContent;
}
void TreeNode::setMessage(std::string message){
    this->message = message;
}
const std::vector<TreeNode*>& TreeNode::getChildren()const{
    return children;
}
void TreeNode::addChildren(TreeNode* child){
    this->children.push_back(child);
}

const time_t& TreeNode::getCreatedTimestamp() const{
    return createdTimestamp;
}

const time_t& TreeNode::getSnapshotTimestamp() const{
    return snapshotTimestamp;
}

void TreeNode::setSnapshotTimestamp(){
    snapshotTimestamp = time(nullptr);
}

void TreeNode::setModifiedTimestamp(){
    modifiedTimestamp = time(nullptr);
}