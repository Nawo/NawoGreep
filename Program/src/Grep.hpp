#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#include "FindedFiles.hpp"

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
    int inFilePatternsNumber = 0;
    bool find = false;

    std::queue<std::filesystem::directory_entry> filesToParse;
    std::vector<FindedFiles> findedFiles;

    std::chrono::time_point<std::chrono::system_clock> startProgramTime;
    std::chrono::time_point<std::chrono::system_clock> endProgramTime;

    void parseArguments(const int& argc, char* argv[]);
    void searchFiles();
    void parseFiles();
    void saveToResultFile();
    void saveToLogFile();
    void printVariables();

public:
    Grep() = delete;
    Grep(const int& argc, char* argv[]);

    ~Grep() = default;

    void run();
};