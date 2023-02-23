#pragma once

#include <filesystem>
#include <string>
#include <thread>

struct FindedFiles {
    // VARIABLES
    std::filesystem::directory_entry filePatch_;
    int inFilePatternsNumber_;
    std::thread::id threadID_;

    // CONTAINERS
    std::vector<std::pair<int, std::string>> lines;

    // GETTERS
    std::filesystem::directory_entry getFilePatch() const;
    int getInFilePatternsNumber() const;
    std::thread::id getThreadID() const;

    // SETTERS
    void setInFilePatternsNumber(const int& _InFilePatternsNumber);

    // CONSTRUCTOR/DESTRUCTOR
    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch, const std::thread::id& threadID);
    ~FindedFiles() = default;
};