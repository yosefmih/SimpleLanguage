#ifndef PARSER_H
#define PARSER_H

#include "../commons/defs.h"

#include <fstream> 
#include <iostream>
#include <vector>
#include <queue>

#include "../ast/astnode.h"
#include "../ast/assignnode.h"
#include "../ast/binaryexprnode.h"
#include "../ast/blocknode.h"
#include "../ast/breakcontinuenode.h"
#include "../ast/exprnode.h"
#include "../ast/fornode.h"
#include "../ast/ifelsenode.h"
#include "../ast/importnode.h"
#include "../ast/lennode.h"
#include "../ast/literalnode.h"
#include "../ast/locnode.h"
#include "../ast/methodargnode.h"
#include "../ast/methodcallnode.h"
#include "../ast/methoddeclnode.h"
#include "../ast/programnode.h"
#include "../ast/returnnode.h"
#include "../ast/statementnode.h"
#include "../ast/unaryexprnode.h"
#include "../ast/vardeclnode.h"
#include "../ast/whilenode.h"
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
         * The syntax is `import <id>;`. The parsing must have been done upto the keyword import.
         * 
         * @param import: the ImportNode node for the import statement
         */
        void parseImport(ImportNode *import);

        /**
         * Parses one method declaration and the parse subtree is stored at method. If it is parsing 
         * a void method (i.e. the method returns void), it assumes only the return type (void) has been parsed. 
         * It expects to see the identifier on the next token. If it is parsing a non void method, it expects that 
         * both the identifier and the opening parenthesis have been parsed. It starts by parsing the arguments.
         * Whether it is parsing a void or non-void method musts be indicated by the second parameter, voidMethod. 
         * 
         * @param method: where the subtree for this method is rooted at 
         * @param voidMethod: boolean value indicating whether it is parsing a void or non-void method
         */
        void parseMethod(MethodDeclNode *method, bool voidMethod);

        /**
         * Parses a sequence of variable declaration separated by commas. The syntax is [<data-type> <id> {\[<int>\]}*]*. 
         * Each varaible parsed has its own VarDeclNode node and that node is added to varsContainer. The first of the varaibles 
         * in the sequence is expected to have been parsed at least partially into var. If the nextToken fetched is open
         * brackets, then var is modified into an array type and added to varsContainer. Otherwise var is considered completely 
         * parsed. varsContainer is the scope wihtin which the var is being declared.
         * 
         * @param varsContainer: the node representing the scope within which the vars are being defined
         * @param var: the NonTerminal for the first var in the chain, at least partially complete 
         */
        void parseVars(ASTNode *varContainer, VarDeclNode *var);

        /**
         * Completes parsing an array variable declaration. The partially complete node for the variable should be in var. 
         * This method expects parsing is done upto the opening brackets. 
         * 
         * @param var: a partially complete node for an array variable
         */
        void parseArrayDecl(VarDeclNode *var);

        /**
         * Complets parsing an array location. It expects location to already contain the identifier. 
         * The syntax for an array location is:
         *      <id> \[ <expr> \]
         *
         * @param location: the node represnting this array location
         */
        void parseArrayIndex(LocNode *location);

        /**
         * Parses a block of code. Blocks are contained in between braces ({}). The format of blocks is 
         * \{ [<variable_decl>; | <method_call>; | <location> <assing_expr>; | <for> | <while> <if_else> | return <expr> | continue;| break;]* \}
         * The parsed subtree is rooted at blockNode. 
         * 
         * @param blockNode: root of parsed subtree for the block
         */
        void parseBlock(BlockNode *blockNode);

        /**
         * Parses a for loop code. The for loop subtree is rooted at the passed parameter forNode. 
         * The format of a for loop is:
         *      for (<id> = <expr>; <expr>; <location> { <assign_expr> | <increment> } ) <block>
         * Expects that the token for 'for' is already processed.
         * 
         * @param forNode: the root of the for loop subtree
         */
        void parseFor(ForNode *forNode);

        /**
         * Parses a while loop code The subtree is rooted at the parameter whileNode. 
         * The syntax for a while loop is:
         *      while (<expr>) <block>
         * Epects the token for while is already processed 
         * 
         * @param whileNode: the root of the while loop subtree
         */
        void parseWhile(WhileNode *whileNode);

        /**
         * Parses an if-elif-else sequence. The subtree is rooted at the passed ifNode. 
         * The syntax for if-elif-else is:
         *      if (<expr>) <block> {elif(<expr>)<block>}* {else <block>}? 
         * Expects the if token is parsed. 
         * 
         * @param ifNode: the node that will be the root of the subtree
         */
        void parseIfElse(IfElseNode *ifNode);

        /**
         * Parses a return statement. The subtree is parsed into the returnNode. 
         * The syntaxt for the a return statement is :
         *      return {<expr>}?;
         * Expectes the return part is already processed. 
         * 
         * @param returnNode: a NonTerminal root of the return statement 
         */
        void parseReturn(ReturnNode *returnNode);

        /**
         * Parses an expr. The subtree parsed is stored in exprNode.
         * The syntax of expr's is:
         *     <location> | <method_call> | <literal> | len (<id>) | <expr> <binary_op> <expr> | [- | !] <expr> | ( <expr> ) 
         *
         * @param exprNode: the root of the subtree representing the expr
         */
        void parseExpr(ExprNode *exprNode); 

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
        void parseExprHelper(ExprNode *expr, std::vector<ExprNode*> terms, std::vector<Token*> ops);

        /**
         * Parses one term of an expression. A term in an expression is a part of the expression that doesn't contain a
         * binary operation
         *
         * @param term: root of the expression term
         */
        void parseTerm(ExprNode *term);

        /**
         * Parses a method call into a subtree with the methodCallNode as a root. 
         * Syntax of a <method_call> is:
         *     <id> ([<expr>]*,);
         * The method expects the <id> to have already been processed at the time of call. 
         * 
         * @param methodCallNode: the root of the subtree for the methodCallNode
         */
        void parseMethodCall(MethodCallNode *MethodCallNode);

        /**
         * Parses an assignment expression into a subtree with root assignExprNode.
         * synatx of <assign_expr>
         *     <location> [[ ++ | --] | [= | += | -= ] <expr>]
         * 
         * @param assignExprNode: root of the generated subtree for the assignment expression
         */
        void parseAssignExpr(AssignNode *assignExprNode);

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
        ProgramNode *program();
};

#endif 