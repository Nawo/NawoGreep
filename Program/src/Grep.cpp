#include "Grep.hpp"

Grep::Grep(){
    std::cout << "[NAWOGREP] Too few arguments to create the object!" << std::endl;
}

Grep::Grep(const int& argc, char* argv[]) {
    parseArguments(argc, argv);
}

Grep::~Grep(){};

void Grep::parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[NAWOGREP] Too few arguments! " << std::endl;
        std::cout << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
    }
    pattern = argv[1];
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-d" || arg == "--dir") {
            startSearchDirection = argv[++i];
        } else if (arg == "-l" || arg == "--log_file") {
            logFile = argv[++i];
        } else if (arg == "-r" || arg == "--result_file") {
            resultFile = argv[++i];
        } else if (arg == "-t" || arg == "--threads") {
            numberOfThreads = std::stoi(argv[++i]);
        } else {
            std::cerr << "[NAWOGREP] Invalid argument: " << arg << std::endl;
            std::cout << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
        }
    }
}

void Grep::searchFiles() {
    start = std::chrono::high_resolution_clock::now();

    std::filesystem::path folderPath(startSearchDirection);
    if (!std::filesystem::exists(folderPath)) {
        std::cerr << "[NAWOGREP] The folder does not exist!" << std::endl;
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
    pattern.insert(0, 1, ' ');
    pattern.push_back(' ');

    while (!filesToParse.empty()) {
        std::ifstream file(filesToParse.front().path().string());
        while (std::getline(file, lineInFile)) {
            if (std::search(lineInFile.begin(), lineInFile.end(), pattern.begin(), pattern.end()) != lineInFile.end()) {
                saveToResultFile(filesToParse.front(), lineNumber, lineInFile);
                patternsNumber++;
                if (find == false) {
                    filesWithPattern++;
                    find = true;
                }
            }
            lineNumber++;
        }
        find = false;
        lineNumber = 0;
        lineInFile.clear();
        filesToParse.pop();
    }
}

void Grep::saveToResultFile(const std::filesystem::directory_entry& file, const int& lineNumber, const std::string& lineString) {
    std::cout << file << ":" << lineNumber << ": " << lineString << std::endl;
}

void Grep::saveToLogFile(const long& threadID, const std::string& fileName) {
}

void Grep::printVariables() {
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Searched files: " << searchedFiles << std::endl;
    std::cout << "Files with pattern: " << filesWithPattern << std::endl;
    std::cout << "Patterns number: " << patternsNumber << std::endl;
    std::cout << "Result file: " << resultFile << std::endl;
    std::cout << "Log file: " << logFile << std::endl;
    std::cout << "Number of threads: " << numberOfThreads << std::endl;
    std::cout << "Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "[ms]"
              << std::endl;
}

void Grep::run() {
    searchFiles();
    parseFiles();
    printVariables();
}