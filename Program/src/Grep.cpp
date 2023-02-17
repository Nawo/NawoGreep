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
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.emplace(entry.path().string());
            std::ifstream file(entry.path().string());
        }
    }
}

void Grep::parseFiles() {
    std::string line;
    int lineNumber = 1;

    while (std::getline(files.front(), line)) {
        if (line.find(pattern) != std::string::npos) {
            std::cout << "Keyword found in file: "
                      << "--here will be file loc.--"
                      << " at line: " << lineNumber << std::endl;
        }
        lineNumber++;
    }
}

void Grep::printVariables() {
    std::cout << "Start directory: " << start_dir << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Result file: " << result_file << std::endl;
    std::cout << "Number of threads: " << num_threads << std::endl;
}

void Grep::run() {
    searchFiles();
    parseFiles();
    printVariables();
}