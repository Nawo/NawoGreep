#include "Grep.hpp"

Grep::Grep(const int& argc, char* argv[]) {
    parseArguments(argc, argv);
}

void Grep::parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[NAWOGREP] Too few arguments! " << std::endl
                  << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
        exit(0);
    }
    pattern = argv[1];
    pattern.insert(0, 1, ' ');
    pattern.push_back(' ');
    for (int i = 2; i < argc; i++) {
        if (argv[i] == "-d" || argv[i] == "--dir") {
            startSearchDirection = argv[++i];
        } else if (argv[i] == "-l" || argv[i] == "--log_file") {
            logFile = argv[++i];
        } else if (argv[i] == "-r" || argv[i] == "--result_file") {
            resultFile = argv[++i];
        } else if (argv[i] == "-t" || argv[i] == "--threads") {
            numberOfThreads = std::stoi(argv[++i]);
        } else {
            std::cout << "[NAWOGREP] Invalid argument: " << argv[i] << std::endl
                      << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
            exit(0);
        }
    }
}

void Grep::getStartTime() {
    startProgramTime = std::chrono::high_resolution_clock::now();
}

void Grep::searchFiles() {
    std::filesystem::path folderPath(startSearchDirection);
    if (!std::filesystem::exists(folderPath)) {
        std::cout << "[NAWOGREP] The folder " << folderPath << " does not exist!" << std::endl;
        exit(0);
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
        searchedFiles++;
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            filesToParse.emplace(entry.path());
        }
    }
}

void Grep::parseFiles() {
    while (true) {
        queueMutex.lock();
        if (filesToParse.empty()) {
            queueMutex.unlock();
            break;
        }

        while (!filesToParse.empty()) {
            std::thread::id thisThreadId = std::this_thread::get_id();
            std::ifstream file(filesToParse.front().path().string());
            while (std::getline(file, lineInFile)) {
                if (std::search(lineInFile.begin(), lineInFile.end(), pattern.begin(), pattern.end()) != lineInFile.end()) {
                    if (find == false) {
                        filesWithPattern++;
                        find = true;
                    }
                    patternsNumber++;
                    inFilePatternsNumber++;
                    findedFiles.emplace_back(filesToParse.front(), lineInFile, lineNumber, inFilePatternsNumber, thisThreadId);
                }
                lineNumber++;
            }
            find = false;
            inFilePatternsNumber = 0;
            lineNumber = 0;
            lineInFile.clear();
            filesToParse.pop();
        }
        queueMutex.unlock();
    }
}

void Grep::processFilesInQueue() {
    for (int i = 0; i < numberOfThreads; i++) {
        threads.push_back(std::thread([&]() { parseFiles(); }));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void Grep::saveToResultFile() {
    std::sort(findedFiles.begin(), findedFiles.end(), [](const auto& lhs, const auto& rhs) { return lhs.inFilePatternsNumber_ < rhs.inFilePatternsNumber_; });
    std::ofstream fileToWriteResuult("../" + resultFile);
    if (fileToWriteResuult.is_open()) {
        for (const auto& a : findedFiles) {
            fileToWriteResuult << a.filePatch_ << ":" << a.lineNumber_ << ": " << a.lineString_ << std::endl;
        }
        fileToWriteResuult.close();
    }
}

void Grep::saveToLogFile() {
    for (auto& thread : threads) {
        // TO DO
    }
}

void Grep::getEndTime() {
    endProgramTime = std::chrono::high_resolution_clock::now();
}

void Grep::printVariables() {
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
    getStartTime();
    searchFiles();
    processFilesInQueue();
    parseFiles();
    saveToResultFile();
    saveToLogFile();
    getEndTime();
    printVariables();
}