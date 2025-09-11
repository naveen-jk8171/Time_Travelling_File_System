#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <ctime>
#include <vector>

class TreeNode{
private:
    int versionId;
    std::string content;
    std::string message;
    time_t createdTimestamp;
    time_t modifiedTimestamp;
    time_t snapshotTimestamp;
    TreeNode* parent;
    std::vector<TreeNode*> children;
public:
    TreeNode(TreeNode* parent, int versionId);
    ~TreeNode();
    std::string getContent();
    std::string getMessage();
    TreeNode* getParent();
    int getVersionId();
    void insertContent(std::string content);
    void updateContent(std::string newContent);
    void setMessage(std::string message);
    const std::vector<TreeNode*>& getChildren() const;
    void addChildren(TreeNode* child);
    const time_t& getCreatedTimestamp()const;
    const time_t& getSnapshotTimestamp()const;
    void setSnapshotTimestamp();
    void setModifiedTimestamp();
};

#endif