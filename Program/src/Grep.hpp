#pragma once

#include <chrono>
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
    int numberOfThreads = 4;

    std::string pattern = "TietoEvry";
    std::string startSearchDirection = ".";
    std::string logFile = "program.log";
    std::string resultFile = "program.txt";

    std::string lineInFile;
    int lineNumber = 1;
    bool find = false;

    std::queue<std::filesystem::directory_entry> filesToParse;
    std::vector<std::pair<long, std::string>> parsedFiles;

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

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