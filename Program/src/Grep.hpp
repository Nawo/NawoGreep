#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class Grep {
private:
    std::string pattern = "TietoEvry";
    std::string start_dir = ".";
    std::string log_file = "program.log";
    std::string result_file = "program.txt";
    size_t num_threads = 4;

    void searchFiles();
    void printVariables();

    void parseArguments(const int& argc, char* argv[]);

public:
    Grep();
    Grep(const int& argc, char* argv[]);

    ~Grep();

    void run();
};