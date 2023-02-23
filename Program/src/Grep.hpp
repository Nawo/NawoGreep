#pragma once

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <stack>
#include <string>
#include <thread>

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

    std::vector<std::filesystem::directory_entry> filesToParse;
    std::vector<FindedFiles> findedFiles;

    std::mutex queueMutex;
    std::vector<std::pair<std::thread, std::thread::id>> threads;
    void processFilesInQueue();

    std::chrono::time_point<std::chrono::system_clock> startProgramTime;
    std::chrono::time_point<std::chrono::system_clock> endProgramTime;

    void parseArguments(const int& argc, char* argv[]);
    void getStartTime();
    void searchFiles();
    void parseFiles();
    void saveToResultFile();
    void saveToLogFile();
    void getEndTime();
    void printVariables();

public:
    Grep() = delete;
    Grep(const int& argc, char* argv[]);

    ~Grep() = default;

    void run();
};