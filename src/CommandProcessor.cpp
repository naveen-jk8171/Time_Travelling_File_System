#include "CommandProcessor.hpp"
#include <sstream>
#include <iostream>
#include <FileSystem.hpp>

//Let F be the total number of files and H be the number of ancestors for a given version. 
//Let k be the number requested for heap-based commands. 
//The time complexities are:
//The complexities below represent the operational time costs. HashMap-based lookups are amortized average-case O(1), while Heap operations are worst-case.
// CREATE: O(log F) Dominated by heap insertions.
// READ: O(1) Based on HashMap lookup.
// INSERT: O(log F) Dominated by heap updates.
// UPDATE: O(log F) Dominated by heap updates.
// SNAPSHOT: O(1) Based on HashMap lookup.
// ROLLBACK: O(1) Based on HashMap lookup or pointer traversal.
// HISTORY: O(H) Traverses parent pointers to the root.
// RECENT_FILES: O(k * log F) k extractions and k insertions from heap.
// BIGGEST_TREES: O(k * log F) k extractions and k insertions from heap.

FileSystem fileSystem;

std::vector<std::string> tokens(std::string inputString){
    std::istringstream iss(inputString);
    std::string word;
    std::vector<std::string> generatedTokens;
    while(iss >> word){
        generatedTokens.push_back(word);
    }
    return generatedTokens;
}

void processCommands(std::vector<std::string> tokens){
    int i = 0, n = tokens.size();
    while(i < n){
        if (tokens[i] == "CREATE"){
            create(tokens, i);
        }else if (tokens[i] == "READ"){
            read(tokens, i);
        }else if (tokens[i] == "INSERT"){
            insert(tokens, i);
        }else if (tokens[i] == "UPDATE"){
            update(tokens, i);
        }else if (tokens[i] == "SNAPSHOT"){
            snapshot(tokens, i);
        }else if (tokens[i] == "ROLLBACK"){
            rollback(tokens, i);
        }else if (tokens[i] == "HISTORY"){
            history(tokens, i);
        }else if (tokens[i] == "RECENT_FILES"){
            recentFileCommand(tokens, i);
        }else if (tokens[i] == "BIGGEST_TREES"){
            biggestTreeCommand(tokens, i);
        }else{
            std::cout << "Command not found..." << std::endl;
            break;
        }
    }
}

void create(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
    }else{
        filename = tokens[i++];
        if (i != tokens.size()){
            i = tokens.size();
            std::cout << "Filename can't have spaces" << std::endl;
            return;
        }
        fileSystem.createFile(filename);
    }
}
void read(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
    }else{
        filename = tokens[i++];
        if (i != tokens.size()){
            i = tokens.size();
            std::cout << "Filename can't have spaces" << std::endl;
            return;
        }
        fileSystem.readFile(filename);
    }
}
void insert(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    std::string content = "";
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
        return;
    }else{
        filename = tokens[i++];
    }
    while(i < tokens.size()){
        content += tokens[i++];
        if (i != tokens.size()){
            content += " ";
        }
    }
    fileSystem.insertInFile(filename, content);
}
void update(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    std::string content = "";
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
        return;
    }else{
        filename = tokens[i++];
    }
    while(i < tokens.size()){
        content += tokens[i++];
        if (i != tokens.size()){
            content += " ";
        }
    }
    fileSystem.updateInFile(filename, content);
}
void snapshot(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    std::string content = "";
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
        return;
    }else{
        filename = tokens[i++];
    }
    while(i < tokens.size()){
        content += tokens[i++];
        if (i != tokens.size()){
            content += " ";
        }
    }
    fileSystem.snapshotFile(filename, content);
}
void rollback(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    int versionID = INT_MAX;
    if (i == tokens.size()){
        std::cout << "<filename> not provided" << std::endl;
        return;
    }else{
        filename = tokens[i++];
    }
    if (i < tokens.size()){
        bool valid = true;
        for (char j : tokens[i]){
            if (j < '0' || j > '9'){
                valid = false;
                break;
            }
        }
        if (valid){
            versionID = std::stoi(tokens[i++]);
            if (i != tokens.size()){
                i = tokens.size();
                std::cout << "Invalid versionID" << std::endl;
                return;
            }
        }else{
            std::cout << "Invalid versionID" << std::endl;
            return;
        }
    }
    fileSystem.rollbackFile(filename, versionID);
}
void history(std::vector<std::string> tokens, int& i){
    i++;
    std::string filename;
    if (i == tokens.size()){
         std::cout << "<filename> not provided" << std::endl;
        return;
    }else{
        filename = tokens[i++];
        if (i != tokens.size()){
            i = tokens.size();
            std::cout << "Filename can't have spaces" << std::endl;
            return;
        }
    }
    fileSystem.historyOfFile(filename);
}
void recentFileCommand(std::vector<std::string> tokens, int& i){
    i++;
    int num;
    if (i == tokens.size()){
        std::cout << "Number of recent files not provided..." << std::endl;
        return;
    }
    bool valid = true;
    for (char j : tokens[i]){
        if (j < '0' || j > '9'){
            valid = false;
            break;
        }
    }
    if (valid){
        num = std::stoi(tokens[i++]);
        if (i != tokens.size()){
            i = tokens.size();
            std::cout << "Invalid number..." << std::endl;
            return;
        }
    }else{
        i = tokens.size();
        std::cout << "Invalid number..." << std::endl;
        return;
    }
    fileSystem.recentFiles(num);
}
void biggestTreeCommand(std::vector<std::string> tokens, int& i){
    i++;
    int num;
    if (i == tokens.size()){
        std::cout << "Number of recent files not provided..." << std::endl;
        return;
    }
    bool valid = true;
    for (char j : tokens[i]){
        if (j < '0' || j > '9'){
            valid = false;
            break;
        }
    }
    if (valid){
        num = std::stoi(tokens[i++]);
        if (i != tokens.size()){
            i = tokens.size();
            std::cout << "Invalid number..." << std::endl;
            return;
        }
    }else{
        i = tokens.size();
        std::cout << "Invalid number..." << std::endl;
        return;
    }
    fileSystem.biggestTrees(num);
}