#ifndef COMMANDPROCESSOR_HPP
#define COMMANDPROCESSOR_HPP

#include <vector>
#include <string>

std::vector<std::string> tokens(std::string inputString);
void processCommands(std::vector<std::string> tokens);
void create(std::vector<std::string> tokens, int& i);
void read(std::vector<std::string> tokens, int& i);
void insert(std::vector<std::string> tokens, int& i);
void update(std::vector<std::string> tokens, int& i);
void snapshot(std::vector<std::string> tokens, int& i);
void rollback(std::vector<std::string> tokens, int& i);
void history(std::vector<std::string> tokens, int& i);
void recentFileCommand(std::vector<std::string> tokens, int& i);
void biggestTreeCommand(std::vector<std::string> tokens, int& i);

#endif