#ifndef TOKEN_H
#define TOKEN_H

#include "../commons/defs.h"

class Token {
    public:
        /**
         * The different types of tokens that can be parsed
         * 
         * FILE_END: the end of the file has been reached 
         * UNSPECIFIED: default type when constructed without type 
         * CHAR_LITERAL: one ASCII char
         * INT_LITERAL: a signed number that fits in 8 bytes
         * BOOLEAN_LITERAL: either 'true' or 'false'
         * STRING_LITERAL: any sequence of chars within double quotes 
         * IDENTIFIER: variable names
         */
        enum TokenType {
            FILE_END, UNSPECIFIED, CHAR_LITERAL, INT_LITERAL, BOOLEAN_LITERAL, STRING_LITERAL, IDENTIFIER
        }; 

        TokenType type;

        // a string representation of the token value
        string value;

        // row and col of the start of the token within the input file 
        // the first character is at row 1 and col 1
        uint32_t row;
        uint32_t col;

        /**
         * Constructs a Token instance of type type, string value of value at row and col 
         * 
         * @param type a TokenType value for this token
         * @param value a string representation for the value this token represents 
         * @param row an unsigned integer representing the row at which this token starts
         * @param col an unsigned integer representing the col at which this token starts 
         */
        Token(TokenType type, string value, uint32_t row, uint32_t col); 

        /**
         * Constructs a Token instance of unspecified type and string value "" at row and col
         * 
         * @param row an unsigned integer representing the row at which this token starts
         * @param col an unsigned integer representing the col at which this token starts 
         */
        Token(uint32_t row, uint32_t col);

        /**
         * Returns true if the type of this token is equal to the passed
         * type.
         */
        bool checkType(TokenType type);

        /**
         * Returns true if the value of this token and the passed value are 
         * equal
         */
        bool checkValue(string value);

        /**
         * Same as isValue(string value) but for chars
         */
        bool checkValue(char value);

        /**
         * Returns true if this token represents a data type in SimpleLanguage
         * Data types are bool, char and int 
         */ 
        bool isDataType();

        // checks to see if t
        bool isAssignOp(); 

        bool isLiteral();

        bool isBinaryOp();

        bool isUnaryOp();

        bool isIncrementOp();

        bool isMorePrecedent(Token *anotherToken); 

        bool isCompoundAssignOp();
        
        /**
         * A string representation of a token instance, with the format 
         * "[type as a string] [value] at [row,col]"
         * 
         * ex: Token(CHAR_LITERAL, "c", 1, 2) -> "CHARLITERAL c at 1,2" 
         */
        operator string() const;

        /**
         * A string rep for the tester
         * 
         * ex Token(CHAR_LITERAL, "c", 1, 2) -> 1 CHARLITERAL 'c'
         */
        string toString() const;

    private:
        /**
         * Gets the token type as a string 
         */
        string getTokenTypeStr() const; 

};

#endif 