#ifndef CLI_H
#define CLI_H

#include <string>

#include "common.h"

class CLI {
    public:
        CLI() {};

        /**
         * DEFAULT: default stage
         * SCAN: scan the input and stop
         * PARSE: scan, parse then stop
         * INTER: produce an intermediate rep and stop
         * ASSEMBLY: produce assembly from the input
        */
        enum Action {DEFAULT, SCAN, PARSE, INTER, ASSEMBLY};

        // list of optimizations to perform
        // static string opts[];

        // Name of file to write output to
        inline static string outfile;

        // Name of file to get input from
        inline static string infile;

        // The type of processing to perform
        inline static Action stage = DEFAULT;

        // Whether to output debugging info
        inline static bool debug;

        static void parse(int argc, string args[]); 

        static void printErrorMsg(string message);
};

#endif