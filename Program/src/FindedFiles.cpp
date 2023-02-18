#include "FindedFiles.hpp"

FindedFiles::FindedFiles(const std::filesystem::directory_entry& filePatch, const std::string& lineString, const int& lineNumber, const int& inFilePatternsNumber, const std::thread::id& threadID)
    : filePatch_(filePatch),
      lineString_(lineString),
      lineNumber_(lineNumber),
      inFilePatternsNumber_(inFilePatternsNumber),
      threadID_(threadID){};

std::filesystem::directory_entry FindedFiles::getFilePatch() {
    return filePatch_;
}
std::string FindedFiles::getLineString() {
    return lineString_;
}
int FindedFiles::getLineNumber() {
    return lineNumber_;
}
int FindedFiles::getInFilePatternsNumber() {
    return inFilePatternsNumber_;
}
std::thread::id FindedFiles::getThreadID() {
    return threadID_;
}