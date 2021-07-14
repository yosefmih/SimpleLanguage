#include "scanner.h"

#include <iostream>
#include <string>
#include <algorithm>

#include "token.h"
#include "../commons/symbols.h"
#include "../commons/keywords.h"
#include "../commons/defs.h"
#include "../commons/exceptions.h"

#define INT_ASCII_START 48 
#define INT_ASCII_END 57 

#define ALPHA_CAP_START 65 
#define ALPHA_CAP_END 90 
#define ALPHA_SMALL_START 97
#define ALPHA_SMALL_END 122 
#define VALID_CHAR_START 32 
#define VALID_CHAR_END 126
#define TAB_SPACE_COUNT 3
   
#define ASCII_CHAR_TO_INT(x) (x - INT_ASCII_START)
#define IS_EMPTY_SPACE(x) (x == '\t' || x == ' ' || x== '\n')

static const char UNDERSCORE = '_';
static const char NEWLINE = '\n';
static const char TAB = '\t';
static const char SPACE = ' ';
static const char FILE_END = 3;

using std::ostream; 
using std::istream;
using std::to_string;

Scanner::Scanner(
    istream &inputStream,
    ostream &debugStream
) : inputStream(inputStream), debugStream(debugStream), debug(true), currentCol(1), currentRow(1), nextChar(0) {}

Scanner::Scanner(
    istream &inputStream
) : inputStream(inputStream), debugStream(std::cout), debug(false), currentCol(1), currentRow(1), nextChar(0) {}

string Scanner::errorMsgPrefix() const {
    return "Error at " + getLocationAsStr() + ":";
}

bool Scanner::isDigit(char val) const {
    return val >= INT_ASCII_START && val <= INT_ASCII_END;
}

bool Scanner::isAlpha(char val) const {
    return (val >= ALPHA_SMALL_START && val <= ALPHA_SMALL_END) ||
           (val >= ALPHA_CAP_START && val <= ALPHA_CAP_END) ||
           val == '_';
}

bool Scanner::isHexChar(char val) const {
    return (val >= 97 && val <= 102) || (val >= 65 && val <= 70);
}

bool Scanner::isValidChar(char val) const {
    return val >= VALID_CHAR_START && val <= VALID_CHAR_END;
}

string Scanner::getLocationAsStr() const {
    return to_string(currentRow) + "," + to_string(currentCol);
}

void Scanner::parseEscChar() {
    consume(false);
    if (nextChar != FILE_END) {
        parsed += '\\';
        switch (nextChar) {
            case '\\':
            case '\'':
            case '\"':
            case 'n':
            case 't':
                parsed += nextChar;
                return;
            default:
                throw ScanningError(errorMsgPrefix() + " wrong char '" + nextChar + "' after escape");
        }
    }

    throw ScanningError(errorMsgPrefix() + " unepected EOF after escape");
}

void Scanner::parseString() {
    if (debug) {
        debugStream << "skipping string at " << currentRow << ", " << currentCol << std::endl;
    }

    while (true) {
        consume(false);
        switch (nextChar) {
            case '\"':
                // end of the string 
                return;
            case TAB:
            case NEWLINE:
            case SPACE:
            case '\'':
                parsed += nextChar;
                break;
            case '\\':
                // start of an esc character 
                try {
                    parseEscChar();
                } catch (ScanningError &se) {
                    throw se; 
                }
                break; 
            default:
                if (isValidChar(nextChar)) {
                    parsed += nextChar;
                } else {
                    throw new ScanningError(errorMsgPrefix() + " unexpected char '" + nextChar + "'");
                }
        }
    }
}

void Scanner::parseChar() {
    if (debug) {
        debugStream << "parsing char at " << currentRow << ", " << currentCol << std::endl;
    }

    while (true) {
        consume(false);
        if (isValidChar(nextChar)) {
            switch (nextChar) {
                case '\'':
                    if (parsed.length() >= 1) {
                        // complete, valid char 
                        return; 
                    } else {
                        throw new ScanningError(errorMsgPrefix() + " empty char");
                    } 
                case '\\':
                    // start of escape char 
                    try {
                        parseEscChar();
                        break;
                    } catch (ScanningError &se) {
                        throw se;
                    }
                default:
                    if (parsed.length() == 0) {
                        // add only if parsed is empty
                        // otherwise the char would be invalid 
                        parsed += nextChar;
                    } else {
                        throw new ScanningError(errorMsgPrefix() + " unexpected character '" + nextChar + "'");
                    }
            }
        } else {
            throw new ScanningError(errorMsgPrefix() + " unexpected character '" + nextChar + "'");
        }
    }
}

void Scanner::parseInt() {
    if (debug) {
        debugStream << "parsing int at " << currentRow << ", " << currentCol << std::endl;
    }

    bool isHex = false; 
    // this can be Hex only if the last char parsed is 0
    bool canBeHex = nextChar == '0';
    while (true) {
        consume(true);
        if (nextChar == 'x') {
            // is Hex if canBeHex is true, i.e. first char parsed is 0 and 
            // parsed has length 1 ('0')
            if (canBeHex && parsed.length() == 1) {
                isHex = true;
                parsed += nextChar; 
            } else {
                throw new ScanningError(errorMsgPrefix() + " unexpected char '" + nextChar + "'");
            }
        } else if (isDigit(nextChar)) {
            parsed += to_string(ASCII_CHAR_TO_INT(nextChar));  
        } else if (isHex && isHexChar(nextChar)) {
            parsed += nextChar; 
        } else {
            // end of int 
            return;
        }
    }
}

void Scanner::parseIdentifier() {
    if (debug) {
        debugStream << "parsing identifier at " << currentRow << ", " << currentCol << std::endl;
    }
    while (true) {
        consume(true);
        if (isAlpha(nextChar)) {
            parsed += nextChar;
        } else if (isDigit(nextChar)) {
            if (parsed.length() > 0) {
                // then the integer is not at the begging of the identifier
                // acceptable 
                parsed += to_string(ASCII_CHAR_TO_INT(nextChar)); 
            } else {
                throw new ScanningError(errorMsgPrefix() + " identifers can't start with a number");
            }
        } else {
            return;
        }
    }
}

void Scanner::skipWhiteSpace() {    
    if (debug) {
        debugStream << "skipping whitespaces at " << currentRow << ", " << currentCol << std::endl;
    }

    while (true) {
        consume(true);
        if (!IS_EMPTY_SPACE(nextChar)) {
            return;
        } 
    }
}

void Scanner::skipComment(bool isSingleLine) {
    if (debug) {
        debugStream << "skipping comments at " << currentRow << ", " << currentCol << " type singleLine " << isSingleLine << std::endl;
    }

    while (true) {
        consume(true);
        std::cerr << "char being processed " << nextChar << std::endl;
        if (isSingleLine && nextChar == '\n') {
            // end of comment 
            return; 
        } else if (!isSingleLine && nextChar == Symbols::OP_TIMES) {
            // could be start of end of the comment
            // loop to account for many OP_TIMES followed by a /
            while (true) {
                consume(true);
                std::cerr << "mlp, char being processed " << nextChar << std::endl;
                if (nextChar == Symbols::OP_DIVIDE) {
                    // "*/"" ends a multi line comment
                    return; 
                } else if (nextChar != Symbols::OP_TIMES) {
                    break;
                }
            }

        } else if (nextChar == FILE_END) {
            // EOF came before the end of the comment, should fail 
            throw new ScanningError(errorMsgPrefix() + " end of line came before end of comment");
        }
    }
}

void Scanner::consume(bool applySkips) {
    inputStream.get(nextChar);
    if (inputStream.eof()) {
        // end of file, need to set this so that individual 
        // parser functions can decide what to do
        nextChar = FILE_END;
    } else if (inputStream.fail()) {
        throw ScanningError("Unable to read from file"); 
    }

    if (nextChar == '\n') {
        currentRow += 1;
        currentCol = 1;
    } else if (nextChar == '\t') {
        currentCol += TAB_SPACE_COUNT;
    } else {
        currentCol += 1;
    }
}

Token *Scanner::getNextToken() {
    if (debug) {
        debugStream << "getting a new token at " << currentRow << ", " << currentCol << std::endl;
    }
    Token *newToken = new Token(currentRow, currentCol);
    parsed = ""; // reset parsed
    while (true) {
        if (nextChar == 0) {
            // nextChar hasn't been loaded yet
            consume(true);
        } 

        if (nextChar == FILE_END) {
            if (debug) {
                debugStream << "end of file reached" << std::endl;
            }
            newToken->type = Token::FILE_END;
            return newToken; 
        } 

        try {
            string value;
            Token::TokenType type = Token::UNSPECIFIED;

            if (isAlpha(nextChar)) {
                parsed += nextChar; 
                parseIdentifier();
                value = parsed;
                // determine if the parsed identifer is one of the reserved keywords
                auto keywordsVec = Keywords::getVec(); 
                if (parsed == Keywords::TRUE || parsed == Keywords::FALSE) {
                    type = Token::BOOLEAN_LITERAL;
                } else if (
                    std::find(keywordsVec.begin(), keywordsVec.end(), parsed) == keywordsVec.end() 
                ) {
                    // parsed is not a member of the reserved words, has to be an identifier
                    type = Token::IDENTIFIER; 
                }
            } else if (isDigit(nextChar)) {
                // beginning of a number
                parsed = to_string(ASCII_CHAR_TO_INT(nextChar));
                parseInt();
                type = Token::INT_LITERAL;
                value = parsed;
            } else {
                char oldChar; // need to save the previous char sometimes
                switch (nextChar) {
                    case Symbols::DOUBLE_QUOTES: // start of a string 
                        parseString();
                        type = Token::STRING_LITERAL;
                        value = '"' + parsed + '"';
                        nextChar = 0; // makes next getNextToken start by getting a char
                        break;
                    case Symbols::SINGLE_QUOTES: // start of a char 
                        parseChar();
                        type = Token::CHAR_LITERAL;
                        value = "'" + parsed + "'";
                        nextChar = 0; 
                        break;
                    case Symbols::OP_PLUS:
                    case Symbols::OP_MINUS:
                        oldChar = nextChar; 
                        parsed += nextChar; 
                        consume(true);
                        if (nextChar == Symbols::EQUAL || oldChar == nextChar) {
                            // gets += -= ++ or -- symbols 
                            parsed += nextChar;
                            nextChar = 0; 
                        }
                        value = parsed; 
                        break;
                    case Symbols::EQUAL:
                    case Symbols::OP_LESS:
                    case Symbols::OP_GREATER:
                    case Symbols::EXCALAMATION:
                        parsed += nextChar;
                        consume(true);
                        if (nextChar == Symbols::EQUAL) {
                            // gets >=, <=, != and ==
                            parsed += nextChar;
                            nextChar = 0;
                        }
                        value = parsed;
                        break;
                    case Symbols::AMPERSAND:
                    case Symbols::PIPE:
                        oldChar = nextChar;
                        parsed = nextChar;
                        consume(true); 
                        if (nextChar == oldChar) {
                            // gets && or || 
                            parsed += nextChar; 
                            value = parsed;
                            nextChar = 0;
                            break;
                        }
                        // | or & aren't recognized as a token 
                        throw ScanningError(errorMsgPrefix() + " unrecognized char '" + nextChar +"'");
                    case Symbols::LPAREN:
                    case Symbols::RPAREN:
                    case Symbols::LBRACE:
                    case Symbols::RBRACE:
                    case Symbols::LBRACKET:
                    case Symbols::RBRACKET: 
                    case Symbols::OP_TIMES:
                    case Symbols::OP_MOD:
                    case Symbols::COMMA:
                    case Symbols::SEMICOLON:
                        value = nextChar; 
                        nextChar = 0;
                        break;
                    case Symbols::OP_DIVIDE:
                        consume(true);
                        if (nextChar == Symbols::OP_DIVIDE) {
                            // start of single line comment 
                            skipComment(true); 
                        } else if (nextChar == Symbols::OP_TIMES) {
                            // start of multi line comment 
                            skipComment(false); 
                        } else {
                            value = Symbols::OP_DIVIDE; 
                            break; 
                        }
                        consume(true);
                        // reset these since the nextToken starts here
                        // and get the next token 
                        newToken->row = currentRow;
                        newToken->col = currentCol;
                        continue;
                    case TAB:
                    case NEWLINE:
                    case SPACE: 
                        skipWhiteSpace();
                        // reset the location for the new Token
                        newToken->row = currentRow;
                        newToken->col = currentCol;
                        continue; 
                    case FILE_END:
                        type = Token::FILE_END;
                        break;
                    default:
                        throw ScanningError(errorMsgPrefix() + " unrecognized char '" + nextChar + "'");
                }
            }

            newToken->type = type;
            newToken->value = value;
            return newToken;
        } catch (Exception &e) {
            throw e;
        }
    }
}