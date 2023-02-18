#pragma once

#include <filesystem>
#include <string>
#include <thread>

struct FindedFiles {
    std::filesystem::directory_entry filePatch_;
    std::string lineString_;
    int lineNumber_;
    int inFilePatternsNumber_;
    std::thread::id threadID_;

    std::filesystem::directory_entry getFilePatch();
    std::string getLineString();
    int getLineNumber();
    int getInFilePatternsNumber();
    std::thread::id getThreadID();

    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch, const std::string& lineString, const int& lineNumber, const int& inFilePatternsNumber, const std::thread::id& threadID);
    ~FindedFiles() = default;
};