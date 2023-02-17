#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

class Grep {
private:
    int searchedFiles = 0;
    int filesWithPattern = 0;
    int patternsNumber = 0;
    std::string pattern = "TietoEvry";
    std::string start_dir = ".";
    std::string log_file = "program.log";
    std::string result_file = "program.txt";
    int num_threads = 4;

    std::queue<std::filesystem::directory_entry> filesToParse;
    std::vector<std::pair<long, std::string>> parsedFiles;

    void parseArguments(const int& argc, char* argv[]);
    void searchFiles();
    void parseFiles();
    void saveToResultFile(const std::filesystem::directory_entry& file, const int& lineNumber, const std::string& lineString);
    void saveToLogFile(const long& threadID, const std::string& fileName);
    void printVariables();

public:
    Grep();
    Grep(const int& argc, char* argv[]);

    ~Grep();

    void run();
};