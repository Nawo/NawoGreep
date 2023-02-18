#pragma once

#include <filesystem>
#include <string>

struct FindedFiles {
    std::filesystem::directory_entry filePatch_;
    std::string lineString_;
    int lineNumber_;
    int inFilePatternsNumber_;
    long threadID_;

    std::filesystem::directory_entry getFilePatch();
    std::string getLineString();
    int getLineNumber();
    int getInFilePatternsNumber();
    long getThreadID();

    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch, const std::string& lineString, const int& lineNumber, const int& inFilePatternsNumber, const long& threadID);
    ~FindedFiles() = default;
};