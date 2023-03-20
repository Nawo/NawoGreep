#include "Grep.hpp"

Grep::Grep(const int& argc, char* argv[]) {
    parseArguments(argc, argv);
}

void Grep::parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "[NAWOGREP] Too few arguments! " << std::endl
                  << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
        exit(0);
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
            std::cerr << "[NAWOGREP] Invalid argument: " << arg << std::endl
                      << "[NAWOGREP] Usage: <pattern> -d <start directory> -l <name of log file> -r <name of result file> -t <number of threads>" << std::endl;
        }
    }
}

void Grep::getStartTime() {
    startProgramTime = std::chrono::high_resolution_clock::now();
}

void Grep::searchFiles() {
    std::filesystem::path folderPath(startSearchDirection);

    if (std::filesystem::exists(folderPath)) {
        if (std::filesystem::is_directory(folderPath)) {
            std::vector<std::filesystem::path*> foldersToSearch{&folderPath};

            while (!foldersToSearch.empty()) {
                auto currentFolder = foldersToSearch.back();
                foldersToSearch.pop_back();
                try {
                    for (const auto& entry : std::filesystem::directory_iterator(*currentFolder)) {
                        if (entry.is_regular_file()) {
                            searchedFiles++;
                            if (entry.path().extension() == ".txt") {
                                filesToParse.emplace_back(entry.path().string());
                            }
                        } else if (entry.is_directory()) {
                            foldersToSearch.emplace_back(new std::filesystem::path(entry.path()));
                        }
                    }
                } catch (const std::filesystem::filesystem_error& ex) {
                    // std::cerr << "[NAWOGREP] " << ex.what() << std::endl;
                }
            }
        } else {
            std::cerr << "[NAWOGREP] argument " << startSearchDirection << " is not direction!";
            exit(0);
        }
    } else {
        std::cerr << "[NAWOGREP] direction " << startSearchDirection << " isn't exist!";
        exit(0);
    }
}

bool operator==(const FindedFiles& lhs, const FindedFiles& rhs) {
    return lhs.getThreadID() == rhs.getThreadID();
}
bool compareObjects(const std::shared_ptr<FindedFiles> a, const std::shared_ptr<FindedFiles> b) {
    return *a < *b;
}

int Grep::countPatternInLine(const std::string lineInFile, const std::string& pattern) {
    int count = 0;
    auto it = std::search(lineInFile.begin(), lineInFile.end(), pattern.begin(), pattern.end());
    while (it != lineInFile.end()) {
        if ((std::find(chars.begin(), chars.end(), *(it - 1)) != chars.end() || it == lineInFile.begin()) &&
            (std::find(chars.begin(), chars.end(), *(it + pattern.size())) != chars.end() || (it + pattern.size()) == (lineInFile.end()))) {
            count++;
        }
        it = std::search(it + pattern.size(), lineInFile.end(), pattern.begin(), pattern.end());
    }
    return count;
}

void Grep::parseFiles() {
    while (true) {
        queueMutex.lock();
        if (filesToParse.empty()) {
            queueMutex.unlock();
            break;
        }
        auto fileToParse = filesToParse.back();
        filesToParse.pop_back();
        queueMutex.unlock();

        bool find = false;
        int inFilePatternsNumber = 0;
        int lineNumber = 1;

        std::shared_ptr<FindedFiles> findedFiles_iterator = nullptr;

        std::thread::id thisThreadId = std::this_thread::get_id();
        std::ifstream file(fileToParse.path());
        std::string lineInFile;

        while (std::getline(file, lineInFile)) {
            auto countPatternsInLine = countPatternInLine(lineInFile, pattern);
            if (countPatternsInLine) {
                if (find == false) {
                    find = true;
                    filesWithPattern++;
                    findedFilesMutex.lock();
                    findedFiles_iterator = parsedFilesWithPattern.emplace_back(std::make_shared<FindedFiles>(fileToParse, thisThreadId));
                    findedFilesMutex.unlock();
                }
                findedFiles_iterator->lines.emplace_back(lineNumber, lineInFile);
                patternsNumber += countPatternsInLine;
                inFilePatternsNumber += countPatternsInLine;
            }
            lineNumber++;
        }
        if (find) {
            findedFiles_iterator->setInFilePatternsNumber(inFilePatternsNumber);
        }
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
    std::sort(parsedFilesWithPattern.begin(), parsedFilesWithPattern.end(), compareObjects);
    std::ofstream fileToWriteResuult(resultFile + ".txt", std::ios::out | std::ios::trunc);
    if (fileToWriteResuult.is_open()) {
        for (const auto& file : parsedFilesWithPattern) {
            for (const auto& element : file->lines)
                fileToWriteResuult << file->getFilePatch()
                                   << ":" << element.first
                                   << ": " << element.second
                                   << std::endl;
        }
        fileToWriteResuult.close();
    }
}

void Grep::saveToLogFile() {
    for (const auto& file : parsedFilesWithPattern) {
        toLogFile.emplace(file->getThreadID(), file->getFilePatch().path().filename().string());
    }

    for (const auto& [threadId, fileName] : toLogFile) {
        threadFileCount[threadId]++;
    }

    for (const auto& [threadId, fileCount] : threadFileCount) {
        sortedThreads.emplace_back(threadId, fileCount);
    }

    std::sort(sortedThreads.begin(), sortedThreads.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::ofstream outFile(logFile + ".log");

    for (const auto& [threadId, fileCount] : sortedThreads) {
        outFile << threadId << ":";
        for (const auto& [tId, fileName] : toLogFile) {
            if (tId == threadId) {
                outFile << " " << fileName;
            }
        }
        outFile << std::endl;
    }
    outFile.close();
}

void Grep::getEndTime() {
    endProgramTime = std::chrono::high_resolution_clock::now();
}

void Grep::printVariables() {
    std::cout << "Searched files: " << searchedFiles << std::endl
              << "Files with pattern: " << filesWithPattern << std::endl
              << "Patterns number: " << patternsNumber << std::endl
              << "Result file: " << resultFile << ".txt" << std::endl
              << "Log file: " << logFile << ".log" << std::endl
              << "Used threads: " << numberOfThreads << std::endl
              << "Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endProgramTime - startProgramTime).count()
              << "[ms]"
              << std::endl;
}

void Grep::run() {
    getStartTime();
    searchFiles();
    processFilesInQueue();
    saveToResultFile();
    saveToLogFile();
    getEndTime();
    printVariables();
}