#include "Grep.hpp"

int main(int argc, char* argv[]) {
    Grep grep(argc, argv);
    grep.run();

    return 0;
}