#include "Grep.hpp"

Grep::Grep(const int& argc, char* argv[]) {
    parseArguments(argc, argv);
}

void Grep::parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[NAWOGREP] Too few arguments! " << std::endl
                  << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
    }
    pattern = argv[1];
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        pattern.insert(0, 1, ' ');
        pattern.push_back(' ');
        if (arg == "-d" || arg == "--dir") {
            startSearchDirection = argv[++i];
        } else if (arg == "-l" || arg == "--log_file") {
            logFile = argv[++i];
        } else if (arg == "-r" || arg == "--result_file") {
            resultFile = argv[++i];
        } else if (arg == "-t" || arg == "--threads") {
            numberOfThreads = std::stoi(argv[++i]);
        } else {
            std::cout << "[NAWOGREP] Invalid argument: " << arg << std::endl
                      << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
        }
    }
}

void Grep::searchFiles() {
    startProgramTime = std::chrono::high_resolution_clock::now();

    std::filesystem::path folderPath(startSearchDirection);
    if (!std::filesystem::exists(folderPath)) {
        std::cout << "[NAWOGREP] The folder does not exist!" << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
        searchedFiles++;
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            filesToParse.emplace(entry.path());
        }
    }
}

void Grep::parseFiles() {
    while (!filesToParse.empty()) {
        std::ifstream file(filesToParse.front().path().string());
        while (std::getline(file, lineInFile)) {
            if (std::search(lineInFile.begin(), lineInFile.end(), pattern.begin(), pattern.end()) != lineInFile.end()) {
                if (find == false) {
                    filesWithPattern++;
                    find = true;
                }
                patternsNumber++;
                inFilePatternsNumber++;
                findedFiles.emplace_back(filesToParse.front(), lineInFile, lineNumber, inFilePatternsNumber, 15);
            }
            lineNumber++;
        }
        find = false;
        inFilePatternsNumber = 0;
        lineNumber = 0;
        lineInFile.clear();
        filesToParse.pop();
    }
}

void Grep::saveToResultFile() {
    std::sort(findedFiles.begin(), findedFiles.end(), [](const auto& lhs, const auto& rhs) { return lhs.inFilePatternsNumber_ < rhs.inFilePatternsNumber_; });
    for (const auto& a : findedFiles) {
        std::cout << a.filePatch_ << ":" << a.inFilePatternsNumber_ << ":" << a.lineNumber_ << ":" << a.threadID_ << ":" << a.lineString_ << std::endl;
    }
}

void Grep::saveToLogFile() {
}

void Grep::printVariables() {
    endProgramTime = std::chrono::high_resolution_clock::now();
    std::cout << "Searched files: " << searchedFiles << std::endl
              << "Files with pattern: " << filesWithPattern << std::endl
              << "Patterns number: " << patternsNumber << std::endl
              << "Result file: " << resultFile << std::endl
              << "Log file: " << logFile << std::endl
              << "Number of threads: " << numberOfThreads << std::endl
              << "Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endProgramTime - startProgramTime).count()
              << "[ms]"
              << std::endl;
}

void Grep::run() {
    searchFiles();
    parseFiles();
    saveToResultFile();
    saveToLogFile();
    printVariables();
}