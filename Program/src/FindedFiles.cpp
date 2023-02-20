#include "FindedFiles.hpp"

FindedFiles::FindedFiles(const std::filesystem::directory_entry& filePatch, const std::thread::id& threadID)
    : filePatch_(filePatch),
      threadID_(threadID){};

std::filesystem::directory_entry FindedFiles::getFilePatch() {
    return filePatch_;
}
int FindedFiles::getInFilePatternsNumber() {
    return inFilePatternsNumber_;
}
std::thread::id FindedFiles::getThreadID() {
    return threadID_;
}