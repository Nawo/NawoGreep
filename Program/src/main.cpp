#include <iostream>
#include <string>

std::string start_dir = ".";
std::string log_file = "program.log";
std::string result_file = "program.txt";
size_t num_threads = 4;

void run() {
    // Run program functions
}

void parseArguments(const int& argc, char* argv[]) {
    if (argc < 2) {
        //print_usage();
        //exit(1);
    }
    for (int i = 1; i < argc; i++) {
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

    std::cout << "Start directory: " << start_dir << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Result file: " << result_file << std::endl;
    std::cout << "Number of threads: " << num_threads << std::endl;
}

int main(int argc, char* argv[]) {
    parseArguments(argc, argv);
    run();

    return 0;
}