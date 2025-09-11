#include <iostream>
#include <string>
#include "CommandProcessor.hpp"
#include "FileSystem.hpp"

int main() {
    std::string line;
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "EXIT") break;
        std::vector<std::string> toks = tokens(line);
        processCommands(toks);
    }
    return 0;
}