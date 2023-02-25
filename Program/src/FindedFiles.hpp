#pragma once

#include <filesystem>
#include <string>
#include <thread>

struct FindedFiles {
    // CONTAINERS
    std::vector<std::pair<int, std::string>> lines;

    // GETTERS
    int getInFilePatternsNumber() const;
    std::filesystem::directory_entry getFilePatch() const;
    std::thread::id getThreadID() const;

    // SETTERS
    void setInFilePatternsNumber(const int& InFilePatternsNumber_);

    // CONSTRUCTOR/DESTRUCTOR
    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch_, const std::thread::id& threadID_);
    ~FindedFiles() = default;

private:
    // VARIABLES
    int inFilePatternsNumber;
    std::filesystem::directory_entry filePatch;
    std::thread::id threadID;
};