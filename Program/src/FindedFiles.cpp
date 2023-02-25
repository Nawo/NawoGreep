#include "FindedFiles.hpp"

FindedFiles::FindedFiles(const std::filesystem::directory_entry& filePatch_, const std::thread::id& threadID_)
    : filePatch(filePatch_),
      threadID(threadID_){};

std::filesystem::directory_entry FindedFiles::getFilePatch() const {
    return filePatch;
}
int FindedFiles::getInFilePatternsNumber() const {
    return inFilePatternsNumber;
}
std::thread::id FindedFiles::getThreadID() const {
    return threadID;
}

void FindedFiles::setInFilePatternsNumber(const int& InFilePatternsNumber_) {
    inFilePatternsNumber = InFilePatternsNumber_;
}