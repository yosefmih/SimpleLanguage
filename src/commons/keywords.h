#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <vector>

#include "defs.h"

class Keywords {
    public:
        inline const static string IMPORT = "import",
                                   BREAK = "break",
                                   CONTINUE = "continue",
                                   IF = "if",
                                   ELIF = "elif",
                                   ELSE = "else",
                                   FOR = "for",
                                   WHILE = "while",
                                   INT = "int",
                                   BOOL = "bool",
                                   CHAR = "char",
                                   VOID = "void",
                                   TRUE = "true",
                                   FALSE = "false",
                                   LEN = "len",
                                   RETURN = "return";
                                   
        static std::vector<string> getVec() {
            return std::vector<string>({IMPORT, BREAK, CONTINUE, IF, ELSE, FOR, WHILE, INT, BOOL, VOID, TRUE, FALSE, LEN, RETURN});
        }
};

// const  std::vector<string> Keywords::keywordsVec = {
//     IMPORT, BREAK, CONTINUE, IF, ELSE, FOR, WHILE, INT, BOOL, VOID, TRUE, FALSE, LEN, RETURN 
// };

#endif
