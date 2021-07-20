#ifndef PARSER_H
#define PARSER_H

#include "../commons/defs.h"

#include <fstream> 
#include <iostream>
#include <vector>
#include <queue>

#include "../commons/keywords.h"
#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "parsetree.h"

class Parser {
    private:
        // the scanner build from the input stream passed to the parser
        Scanner *scanner;

        // used to write debug informaton to when debug is true
        std::ostream &debugStream;
        
        bool debug;

        // root of parse tree 
        NonTerminal *start;

        // fifo queue of tokens that have been obtained from scanner but not parsed
        // subsequent invocations of consumeToken get tokens from here instead of the scanner 
        std::queue<Token*> tokensQueue;

        inline const static uint32_t IMPORT_PHASE = 0;
        inline const static uint32_t VAR_DECLRS_PHASE = 1;
        inline const static uint32_t POST_VARS_PHASE = 2;
        // set to false once a non import statement is processed
        uint32_t phase; 

        /**
         * Prints the passed string debugMsg to the debugStream field if debug is set to true
         */
        void printDebug(string debugMsg);

        /**
         * Parses an import statement. All import statements must come before anything else in the program at the top. 
         * The syntax is `import <id>;`. Tjhe parsing must have been done upto the keyword import.
         * 
         * @param import: the NonTerminal node for the import statement
         */
        void parseImport(NonTerminal *import);

        /**
         * Parses one method declaration and the parse subtree is stored at method. If it is parsing 
         * a void method (i.e. the method returns void), it assumes only the return type (void) has been parsed. 
         * It expects to see the identifier on the next token. If it is parsign a non void method, it expects that 
         * both the identifier and the opening parenthesis have been parsed. It starts by parsing the arguments Whether it is parsing
         * a void or non-void method musts be indicated by the second parameter, voidMethod. 
         * 
         * @param method: where the subtree for this method is rooted at 
         * @param voidMethod: boolean value indicating whether it is parsing a void or non-void method
         */
        void parseMethod(NonTerminal *method, bool voidMethod);

        /**
         * Parses the arguments in a function definition. Method arguments have the structure ([<data_type> <id>]*,). 
         * It expects the opening parenthesis has already been parsed. It adds all the parse tree nodes corresponding to the
         * arguments as children to methodArgs. 
         * 
         * @param methodArgs: a NonTerminal that is going to be the parent node for the arguments
         */
        void parseMethodArgs(NonTerminal *methodArgs);

        /**
         * Parses a sequence of variable declaration separated by commas. The syntax is [<data-type> <id> {\[<int>\]}*]*. 
         * Each varaible parsed has its own NonTerminal node and that node is added to varsContainer. The first of the varaibles 
         * in the sequence is expected to have been parsed at least partially parsed into newVar. If the nextToken fetched is open
         * brackets, then newVar is modified into an array type and added to varsContainer. Otherwise newVar is considered completely 
         * parsed.
         * 
         * @param varsContainer: the NonTerminal node that will ahve all the vars in the chain added as children
         * @param newVar: the NonTerminal for the first var in the chain, at least partially complete 
         */
        void parseVars(NonTerminal *varsContainer, NonTerminal *newVar);

        /**
         * Completes parsing an array variable declaration. The partially complete node for the variable should be in var. 
         * This method expects parsing is done upto the opening brackets. 
         * 
         * @param var: a partially complete node for an array variable
         */
        void parseArrayDecl(NonTerminal *var);

        void parseArrayIndex(NonTerminal *location);

        /**
         * Parses a block of code. Blocks are contained in between braces ({}). The format of blocks is 
         * \{ [<variable_decl>; | <method_call>; | <location> <assing_expr>; | <for> | <while> <if_else> | return <expr> | continue;| break;]* \}
         * The parsed subtree is rooted at blockNode. 
         * 
         * @param blockNode: root of parsed subtree for the block
         */
        void parseBlock(NonTerminal *blockNode);

        /**
         * Parses a for loop code. The for loop subtree is rooted at the passed parameter forNode. 
         * The format of a for loop is:
         *      for (<id> = <expr>; <expr>; <location> { <assign_expr> | <increment> } ) <block>
         * Expects that the token for 'for' is already processed.
         * 
         * @param forNode: a NonTerminal root of the for loop subtree
         */
        void parseFor(NonTerminal *forNode);

        /**
         * Parses a while loop code The subtree is rooted at the parameter whileNode. 
         * The syntax for a while loop is:
         *      while (<expr>) <block>
         * Epects the token for while is already processed 
         * 
         * @param whileNode: the NonTerminal node that will be the root for the subtree
         */
        void parseWhile(NonTerminal *whileNode);

        /**
         * Parses an if-elif-else sequence. The subtree is rooted at the passed ifNode. 
         * The syntax for if-elif-else is:
         *      if (<expr>) <block> {elif(<expr>)<block>}* {else <block>}? 
         * Expects the if token is parsed
         * 
         * @param ifNode: the NonTerminal that will be the root of the subtree
         */
        void parseIfElse(NonTerminal *ifNode);

        /**
         * Parses a return statement. The subtree is parsed into the returnNode. 
         * The syntaxt for the a return statement is :
         *      return {<expr>}?;
         * Expectes the return part is already processed. 
         * 
         * @param returnNode: a NonTerminal root of the return statement 
         */
        void parseReturn(NonTerminal *returnNode);

        /**
         * Parses an expr. The subtree parsed is stored in exprNode.
         * The syntax of expr's is:
         *     <location> | <method_call> | <literal> | len (<id>) | <expr> <binary_op> <expr> | [- | !] <expr> | ( <expr> ) 
         *
         * @param exprNode: the NonTerminal root of the subtree representing the expr
         */
        void parseExpr(NonTerminal *exprNode); 

        /**
         * A function called by parseExpr to assist with binary operation precedence. It recursively compares precedence of the binary ops
         * to determine the right order of terms in the expr. The binary ops should be given in ops and the terms should be given in terms. 
         * A binary op at ops[i] is taken to be the operation between terms[i] and terms[i+1]. The size of ops should be one less than that of terms. 
         *  
         * Example: term1 + term2 * term3 => tems(term1, term2, term3), ops(+, *) => expr(term1, +, expr(term2, *, term3))
         * 
         * @param expr: the root node for the expression 
         * @param terms: an ordered vector of terms in the expression 
         * @param ops: an ordered vector of binary operations. Must be one shorter than terms. 
         */
        void parseExprHelper(NonTerminal *expr, std::vector<NonTerminal*> terms, std::vector<Token*> ops);

        /**
         * Parses one term of an expression. A term in an expression is a part of the expression that doesn't contain a
         * binary operation
         *
         * @param term: root of the expression term
         */
        void parseTerm(NonTerminal *term);

        /**
         * Parses a method call or assignment expression. This function looks ahead in the token stream
         * to decide whether to call parseMethodCall or parseAssignExpr.
         * 
         * @param methodCallOrAssignment: root of the method call or assignment tree
         */
        void parseMethodCallOrAssignment(NonTerminal *methodCallOrAssignment, Token *nextToken);
        
                /**
         * Parses a method call into a subtree with the methodCallNode as a root. 
         * Syntax of a <method_call> is:
         *     <id> ([<expr>]*,);
         * The method expects the <id> to have already been processed at the time of call. 
         * 
         * @param methodCallNode: the root of the subtree for the methodCallNode
         */
        void parseMethodCall(NonTerminal *methodCallNode);

        /**
         * Parses an assignment expression into a subtree with root assignExprNode.
         * synatx of <assign_expr>
         *     <location> = <expr>; 
         * 
         * @param assignExprNode: root of the generated subtree for the assignment expression
         */
        void parseAssignExpr(NonTerminal *assignExprNode);

        /**
         * Returns the next token to be processed. The next token is obtained from toksensQueue if the queue is not empty.
         * If the queue is empty, it is obtained from the scanner object. If deleteToken is true and the token is obtained from
         * the queue, then the token is deleted from tokens. If deleteToken is false and the token is obtained from the scanner, it 
         * is saved on the queue.
         * 
         * @param deleteToken: a boolean determining if the token be deleted from tokens in the case it is returned from the queue
         */
        Token *consumeToken(bool deleteToken);

        /**
         *  This version of consumeToken never saves a returned token: the pointer moves forward. 
         */
        Token *consumeToken();

        /**
         * Shrinks tokensQueue by one if not empty. It does nothing if not empty. 
         */
        void popFromTokens();

    public:
        Parser(std::istream& inputStream, std::ostream& debugStream);

        Parser(std::istream& inputStream);

        ~Parser();
        
        /**
         * Parses the SimpleProgram loaded at time of construction. It assumes the stream stays
         * open and unread. It returns the root of the constructed parse tree if it parses the tree 
         * successfully. It throws a ParsingError if the file can't be parsed. 
         * 
         * @return: an instance of NonTerminal that is the root of the parsed tree
         */
        NonTerminal *program();
};

#endif 