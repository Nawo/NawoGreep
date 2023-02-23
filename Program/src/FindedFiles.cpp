#include "FindedFiles.hpp"

FindedFiles::FindedFiles(const std::filesystem::directory_entry& filePatch, const std::thread::id& threadID)
    : filePatch_(filePatch),
      threadID_(threadID){};

std::filesystem::directory_entry FindedFiles::getFilePatch() const {
    return filePatch_;
}
int FindedFiles::getInFilePatternsNumber() const {
    return inFilePatternsNumber_;
}
std::thread::id FindedFiles::getThreadID() const {
    return threadID_;
}

void FindedFiles::setInFilePatternsNumber(const int& _InFilePatternsNumber) {
    inFilePatternsNumber_ = _InFilePatternsNumber;
}