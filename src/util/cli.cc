#include "cli.h"

#include <iostream> 
#include <cctype>
#include <algorithm>

#include "../commons/defs.h"
#include "../commons/exceptions.h"
#include "../commons/common.h"

// CLI::CLI() {
//     CLI::outfile = "";
//     CLI::infile = "";
//     CLI::stage = DEFAULT;
// }

void CLI::printErrorMsg(string message) {
    std::cerr << message << std::endl;
}

void CLI::parse(int argc, string args[]) {
    string stageStr;
    for (int i = 0; i < argc; ++i) {
        if (args[i] == "--debug" || args[i] == "-d") {
            CLI::debug = true;
        } else if (args[i].rfind("--infile=", 0) == 0) {
            CLI::infile = args[i].substr(9);
        } else if (args[i] == "-i") {
            if (i < argc - 1) {
                CLI::infile = args[i+1];
                i++;
            } else {
                printErrorMsg("No input file specified");
                throw IllegalArgumentException("Input file not specified");
            }
        } else if (args[i].rfind("--outfile=", 0) == 0) {
            CLI::outfile = args[i].substr(10);
        } else if (args[i] == "-o") {
            if (i < argc - 1) {
                CLI::outfile = args[i+1];
                i++;
            } else {
                printErrorMsg("No output file specified");
                throw IllegalArgumentException("Output file not specified");
            }
        } else if (args[i].rfind("--stage=") == 0) {
            stageStr = args[i].substr(8);
        } else if (args[i] == "-s") {
            if (i < (argc - 1)) {
                stageStr = args[i+1];
                i++;
            } else {
                printErrorMsg("No stage specified");
                throw IllegalArgumentException("Stage not specified");
            }
        }
    }

    if (stageStr != "") {
        // transform to lowercase 
        toLower(stageStr);
        if (stageStr == "scan") CLI::stage = SCAN; 
        else if (stageStr == "parse") CLI::stage = PARSE;
        else if (stageStr == "inter") CLI::stage = INTER;
        else if (stageStr == "assembly") CLI::stage = ASSEMBLY;
        else {
            printErrorMsg("Invalid stage: " + stageStr);
            throw IllegalArgumentException("Invalid stage: '" + stageStr + "'");
        }
    }
} 