#include "Grep.hpp"

Grep::Grep(){};
Grep::~Grep(){};

Grep::Grep(const int& argc, char* argv[]) {
    parseArguments(argc, argv);
}

void Grep::parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        //print_usage();
        //exit(1);
    }
    pattern = argv[1];
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-d" || arg == "--dir") {
            start_dir = argv[++i];
        } else if (arg == "-l" || arg == "--log_file") {
            log_file = argv[++i];
        } else if (arg == "-r" || arg == "--result_file") {
            result_file = argv[++i];
        } else if (arg == "-t" || arg == "--threads") {
            num_threads = std::stoi(argv[++i]);
        } else {
            std::cerr << "Invalid argument: " << arg << std::endl;
        }
    }
}

void Grep::searchFiles() {
    std::filesystem::path folderPath(start_dir);
    if (!std::filesystem::exists(folderPath)) {
        std::cerr << "The folder does not exist!" << std::endl;
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

    std::string lineInFile;
    int lineNumber = 1;
    bool find = false;

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
    std::cout << "Searched files: " << searchedFiles << std::endl;
    std::cout << "Files with pattern: " << filesWithPattern << std::endl;
    std::cout << "Patterns number: " << patternsNumber << std::endl;
    std::cout << "Result file: " << result_file << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Number of threads: " << num_threads << std::endl;
    std::cout << "Elapsed time: "
              << "TO DO" << std::endl;
}

void Grep::run() {
    searchFiles();
    parseFiles();
    printVariables();
}