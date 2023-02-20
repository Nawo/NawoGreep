#pragma once

#include <filesystem>
#include <string>
#include <thread>

struct FindedFiles {
    std::filesystem::directory_entry filePatch_;
    int inFilePatternsNumber_;
    std::thread::id threadID_;

    std::vector<std::pair<int, std::string>> lines;

    std::filesystem::directory_entry getFilePatch();
    int getInFilePatternsNumber();
    std::thread::id getThreadID();

    FindedFiles() = delete;
    FindedFiles(const std::filesystem::directory_entry& filePatch, const std::thread::id& threadID);
    ~FindedFiles() = default;
};