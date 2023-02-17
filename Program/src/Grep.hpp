#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

class Grep {
private:
    std::string pattern = "TietoEvry";
    std::string start_dir = ".";
    std::string log_file = "program.log";
    std::string result_file = "program.txt";
    size_t num_threads = 4;

    std::queue<std::ifstream> files;

    void parseArguments(const int& argc, char* argv[]);
    void searchFiles();
    void parseFiles();
    void printVariables();

public:
    Grep();
    Grep(const int& argc, char* argv[]);

    ~Grep();

    void run();
};