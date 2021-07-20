#include <iostream>
#include <cstdint>
#include <fstream>

#include "../commons/common.h"
#include "../util/cli.h"
#include "../commons/exceptions.h"
#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "../parser/parser.h"

int main(int argc, char *argv[]) {
    // std::cout << "Compiling is fun " << argc << std::endl;

    if (argc < 2) return 0;

    string new_argv[argc-1]; 
    for (int i = 1; i < argc; i++) {
        new_argv[i-1] = argv[i]; 
        // std::cout << argv[i] << " " << i << std::endl;
    }

    CLI cli = CLI();
    try {
        cli.parse(argc-1, new_argv);
        std::ifstream input;
        input.open(cli.infile, std::ifstream::in);
        if (cli.stage == cli.SCAN) {
            // perform scan 
            // std::cout << "input file " << cli.infile << std::endl;
            Scanner *scanner;
            
            if (cli.debug) {
                scanner = new Scanner(input, std::cerr);
            } else {
                scanner = new Scanner(input);
            }

            Token *token = scanner->getNextToken();
            while (token->type != Token::FILE_END) {
                std::cout << token->toString() << std::endl; 
                token = scanner->getNextToken();
            }
            input.clear();
            input.close();
            delete scanner;
        } else if (cli.stage == cli.PARSE) {
            // perform parse
            Parser *parser;
            if (cli.debug) {
                parser = new Parser(input, std::cerr);
            } else {
                parser = new Parser(input);
            }
            parser->program();
        }
    } catch (Exception &e) {
        std::cerr << cli.infile << ": " << e.what() << std::endl;
        return 1;
    }

    return 0;
}