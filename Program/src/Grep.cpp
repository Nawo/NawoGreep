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
            std::cout << "[NAWOGREP] Invalid argument: " << arg << std::endl
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
            std::vector<std::filesystem::path> foldersToSearch{folderPath};

            while (!foldersToSearch.empty()) {
                auto currentFolder = foldersToSearch.back();
                foldersToSearch.pop_back();
                for (const auto& entry : std::filesystem::directory_iterator(currentFolder)) {
                    if (entry.is_regular_file()) {
                        std::cout << "ANY: " << entry << "\n";
                        searchedFiles++;
                        if (entry.path().extension() == ".txt") {
                            std::cout << ".TXT: " << entry << "\n";
                            filesToParse.emplace_back(entry.path().string());
                        }
                    } else if (entry.is_directory()) {
                        foldersToSearch.emplace_back(entry.path());
                    }
                }
            }
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
            std::ifstream file(filesToParse.back().path());
            while (std::getline(file, lineInFile)) {
                if (std::search(lineInFile.begin(), lineInFile.end(), pattern.begin(), pattern.end()) != lineInFile.end()) {
                    // TO DO sprawdzanie prefixow i sufixow patternie
                    if (find == false) {
                        filesWithPattern++;
                        find = true;
                        findedFiles.emplace_back(filesToParse.back(), thisThreadId);
                    }
                    patternsNumber++;
                    inFilePatternsNumber++;
                    findedFiles.back().lines.emplace_back(lineNumber, lineInFile);
                }
                lineNumber++;
            }
            findedFiles.back().setInFilePatternsNumber(inFilePatternsNumber);
            find = false;
            inFilePatternsNumber = 0;
            lineNumber = 0;
            filesToParse.pop_back();
            filesToParse.shrink_to_fit();
        }
        queueMutex.unlock();
    }
}

void Grep::processFilesInQueue() {
    for (int i = 0; i < numberOfThreads; i++) {
        threads.push_back(std::make_pair(std::thread([&]() {
                                             parseFiles();
                                         }),
                                         std::this_thread::get_id()));
    }

    for (auto& thread : threads) {
        thread.first.join();
    }
}

void Grep::saveToResultFile() {
    std::sort(findedFiles.begin(), findedFiles.end(), [](const auto& lhs, const auto& rhs) { return lhs.getInFilePatternsNumber() > rhs.getInFilePatternsNumber(); });
    std::ofstream fileToWriteResuult(resultFile, std::ios::out | std::ios::trunc);
    if (fileToWriteResuult.is_open()) {
        for (const auto& file : findedFiles) {
            for (const auto& element : file.lines)
                fileToWriteResuult << file.filePatch_ << ":" << element.first << ": " << element.second << std::endl;
        }
        fileToWriteResuult.close();
    }
}

void Grep::saveToLogFile() {
    // sorted from the thread id with the most files number and ending with the least, (here are only file names, not full paths to files).

    // 140035841935104: file6, file 4
    // 140035825149696: file2
    // 140035833542400:
    // 140035855147214:
    std::multimap<std::thread::id, std::string> toLogFile;
    for (auto& thread : threads) {
        for (const auto& file : findedFiles) {
            toLogFile.emplace(thread.second, file.getFilePatch().path().filename().string());
            // std::cout << thread.second << ": " << file.filePatch_.path().filename().string() << std::endl;
        }
    }
    for (auto& a : toLogFile) {
        std::cout << a.first << ": " << a.second << std::endl;
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
    parseFiles();
    saveToResultFile();
    saveToLogFile();
    getEndTime();
    printVariables();
}