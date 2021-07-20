#ifndef SCANNER_H
#define SCANNER_H

#include <fstream> 
#include <iostream> 

#include "../commons/defs.h"
#include "token.h"

using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;

class Scanner {
    // the file stream being read from
    istream &inputStream;

    // the row and column numbers of the current to be read next
    uint32_t currentRow;
    uint32_t currentCol;

    // a string representation of the token value being parsed
    string parsed;

    // the char that will be processed next
    char nextChar;

    // mode is debug if this is true
    bool debug; 

    // debug information is written to this stream
    ostream &debugStream;

    /**
     * Sets whether to scan in debug mode or not. The mode is set to debug
     * if the parameter debug is true. 
     * Debug mode prints extra information
     * 
     * @param debug: boolean indicating whether the mode is debug or not
     */
    void setTrace(bool debug); 

    /**
     * Returns a prefix for error messages that is shared by 
     * a variety of erros that are thrown 
     */ 
    string errorMsgPrefix() const; 

    /**
     * Returns true if the char val represents an integer in ASCII
     *  
     * @param val: a char value
     */ 
    bool isDigit(char val) const; 

    /**
     * Returns true if val is a valid hex letter
     * valid hex letters are 'a'. 'b', 'c', 'd', 'e', 'f' or
     * the capital version of these
     * 
     * @param val: a char value
     */
    bool isHexChar(char val) const;

    /**
     * Returns true if the char val represents an alphabetical symbol or 
     * the underscore (_) in ASCII
     * 
     * @param val: a char value
     */ 
    bool isAlpha(char val) const;

    /**
     * Returns true if the char val is a valid character for a SimpleLanguage program. 
     * A valid character is one in between 32 and 126
     * 
     * @param val: a char value
     */
    bool isValidChar(char val) const; 


    /**
     * Parses escape characters. This function should be called after the forward slash ('\')
     * demarcatig the start of the escape character is already processed. 
     * Escape characters are '\n', '\r', '\t' and '\\'.
     */
    void parseEscChar();  
 
    /**
     * Parses a string. A string in SimpleLanguage is any sequence of characters within 
     * double quotes. This function should be called after the opening quote is processed. 
     */
    void parseString();

    /**
     * Parses a char. A char is between two single quotes. This function must be called 
     * after processing the opening quote. 
     */
    void parseChar(); 

    /**
     * Parses an integer. 
     */
    void parseInt();

    /**
     * Parses an identifier. Identifiers are a sequence of alphanumeric and underscore chars
     * 
     */
    void parseIdentifier();

    /**
     * Skips white spaces in the input file. 
     */
    void skipWhiteSpace();

    /**
     * Skips comments. Comments can be either single line or multiline. Single line comments 
     * start with "//" and end at the end of the line. Multi line comments start with "/" followed by "*" and 
     * ends with '*' followed by '/'. The calling function must know what type of comments are being skipped 
     * and specificy it in isSingleLine.
     * 
     * @param isSingleLine: bool, true if the comment being skipped is single line 
     */
    void skipComment(bool isSingleLine);


    /**
     * Gets the next character in the input file. If applySkips is true, it gets the next char that is not
     * a new line or tab character.
     * 
     * @param applySkips: bool determining whether to skip new line or tab chars
     */
    void consume(bool applySkips);

    public:
        // Scanner();

        Scanner(istream &inputStream, ostream &debugStream);

        Scanner(istream &inputStream);

        /**
         * Returns a string formatting of the location, row and column, in the file being 
         * scanned. The format is "[row],[col]"
         */
        string getLocationAsStr() const;

        /**
         * Returns a Token instance representing the token read next in the input file 
         */
        Token *getNextToken(); 
};

#endif 