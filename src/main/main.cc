#include <iostream>
#include <cstdint>

#include "../commons/common.h"
#include "../util/cli.h"
#include "../commons/exceptions.h"

int main(int argc, char *argv[]) {
    std::cout << "Compiling is fun " << argc << std::endl;

    if (argc < 2) return 0;

    string new_argv[argc-1]; 
    for (uint32_t i = 1; i < argc; i++) {
        new_argv[i-1] = argv[i]; 
    }

    CLI cli = CLI();
    try {
        cli.parse(argc-1, new_argv);
        if (cli.stage == cli.SCAN) {
            // perform scan 
        } else if (cli.stage == cli.PARSE) {
            // perform parse
        }
    } catch (IllegalArgumentException &e) {
        std::cerr << cli.infile << e.what() << std::endl;
    }

    return 0;
}