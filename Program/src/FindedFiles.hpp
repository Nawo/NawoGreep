#pragma once

#include <filesystem>
#include <string>

struct FindedFiles {
    std::filesystem::directory_entry filePatch_;
    std::string lineString_;
    int lineNumber_;
    int inFilePatternsNumber_;
    long threadID_;

    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch, const std::string& lineString, const int& lineNumber, const int& inFilePatternsNumber, const long& threadID)
        : filePatch_(filePatch),
          lineString_(lineString),
          lineNumber_(lineNumber),
          inFilePatternsNumber_(inFilePatternsNumber),
          threadID_(threadID){};
    ~FindedFiles() = default;
};