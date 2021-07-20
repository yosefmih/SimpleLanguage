#include "parser.h"

#include <iostream> 
#include <fstream>
#include <string>

#include "parsetree.h"
#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "../commons/keywords.h"
#include "../commons/symbols.h"
#include "../commons/exceptions.h"

Parser::Parser(std::istream& inputStream, std::ostream& debugStream) : debug(true), debugStream(debugStream), phase(IMPORT_PHASE) {
    this->scanner = new Scanner(inputStream, debugStream);
    start = new NonTerminal("START"); 
}

Parser::Parser(std::istream& inputStream) : debug(false), debugStream(std::cerr), phase(IMPORT_PHASE) {
    this->scanner = new Scanner(inputStream);
    start = new NonTerminal("START");
}

Parser::~Parser() {
    Token *token;
    for (int i; i < tokensQueue.size(); i++) {
        token = tokensQueue.front();
        tokensQueue.pop();
        delete token;
    }

    delete scanner;
}
void Parser::printDebug(string msg) {
    if (debug) {
        std::cout << msg << " " + scanner->getLocationAsStr() << std::endl;
    }
}

void Parser::parseImport(NonTerminal *import) {
    if (debug) {
        debugStream << "Parsing an import at " << scanner->getLocationAsStr() << std::endl;
    }

    if (phase != IMPORT_PHASE) {
        throw ParsingError("Invalid import at " + scanner->getLocationAsStr() + ". All import statements must be at the top."); 
    }

    Token *nextToken = consumeToken();
    // the token following an import must be an identifier 
    if (!nextToken->checkType(Token::IDENTIFIER)) {
        throw ParsingError("Invalid import at " + scanner->getLocationAsStr() + ". All import statements must be at the top."); 
    }
    import->addChild(nextToken);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Invaid import at " + scanner->getLocationAsStr() + ". All imports must end with ;");
    }
}


void Parser::parseArrayDecl(NonTerminal *var) {
    printDebug("Parsing an array decl");

    Token *nextToken = consumeToken();
    if (!nextToken->checkType(Token::INT_LITERAL)) {
        // there must be an int in between the brackets
        throw ParsingError("Array vars must have an int in between the brackets");
    }
    var->addChild(nextToken);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RBRACKET)) {
        throw ParsingError("Array vars must end with a closng bracket");
    }
    var->addChild(nextToken);
}


void Parser::parseVars(NonTerminal *container, NonTerminal *var) {
    printDebug("Parsing variable declarations");

    Token *nextToken = consumeToken();
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        // this is an array
        // <data type> <id>[<int>]
        var->addChild(nextToken);
        parseArrayDecl(var);
        nextToken = consumeToken();
    } 
    container->addChild(var);

    // parse more linked up vars
    while (nextToken->checkValue(Symbols::COMMA)) {
        NonTerminal *newVar = new NonTerminal("var_decl");
        nextToken = consumeToken();
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Chained variable declaration must have identifier");
        }
        newVar->addChild(nextToken);

        nextToken = consumeToken();
        if (nextToken->checkValue(Symbols::LBRACKET)) {
            parseArrayDecl(newVar);
            nextToken = consumeToken();
        }
    }

    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Unexpected token " + string(*nextToken) + " while parsing variable declarations");
    }
}

void Parser::parseMethod(NonTerminal *method, bool voidMethod) {
    printDebug("Parsing a method declaration");

    Token *nextToken;
    if (voidMethod) {
        nextToken = consumeToken();
        // need to fetch id and lparen before the non void methods and this are equal
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Type must be followed by identifier for method declaration"); 
        }
        method->addChild(nextToken);

        // expected to be '(' 
        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::LPAREN)) {
            throw ParsingError("Expected '(' after identifier in method declaration");
        }
        // method->addChild(nextToken);
    }

    NonTerminal *methodArgs = new NonTerminal("method_args");
    // should have parsed <data-type> <id> '(' at this point
    parseMethodArgs(methodArgs);
    method->addChild(methodArgs);

    NonTerminal *methodBlock = new NonTerminal("method_block");
    parseBlock(method);
    method->addChild(methodBlock);
}

void Parser::parseMethodArgs(NonTerminal *methodArgs) {
    printDebug("Parsing the arguments for in a method declaration");

    // expects the opening parehthesis is already parsed
    Token *nextToken = consumeToken();
    while (!nextToken->checkValue(Symbols::RPAREN)) {
        NonTerminal *arg = new NonTerminal("method_arg");
        if (!nextToken->isDataType()) {
            throw ParsingError("Expected a type in arguments");
        }
        arg->addChild(nextToken);

        nextToken = consumeToken();
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Expected an identifier in arguments");
        }
        arg->addChild(nextToken);
        methodArgs->addChild(arg);

        nextToken = consumeToken();
        if (nextToken->checkValue(Symbols::COMMA)) {
            nextToken = consumeToken();
            if (nextToken->checkValue(Symbols::RPAREN)) {
                throw ParsingError("Expected another argument after ',', but got " + string(*nextToken));
            }
            continue;
        } else if (!nextToken->checkValue(Symbols::RPAREN)) {
            throw ParsingError("Unrecognized token " + string(*nextToken));
        }
    }
}

void Parser::parseBlock(NonTerminal *blockNode) {
    printDebug("Parsing a block");

    uint32_t blockPhase = VAR_DECLRS_PHASE;

    Token *nextToken = consumeToken();
    // must start with '{'
    if (!nextToken->checkValue(Symbols::LBRACE)) {
        throw ParsingError("Blocks must start with '{'");
    } 

    nextToken = consumeToken();
    while (!nextToken->checkValue(Symbols::RBRACE)) {
        if (nextToken->isDataType()) {
            // this has to be a variable declaration
            if (blockPhase != VAR_DECLRS_PHASE) {
                throw ParsingError("all variable declaration must be at the top of a block");
            }

            NonTerminal *var = new NonTerminal("var_decl");
            var->addChild(nextToken);

            nextToken = consumeToken();
            if (!nextToken->checkType(Token::IDENTIFIER)) {
                throw ParsingError("Identifier must come after type in variable declaration");
            }
            var->addChild(nextToken);
            
            parseVars(blockNode, var); 
        } else if (nextToken->checkType(Token::IDENTIFIER)) {
            // either  <location> <assign_expr>; or <method_call>;
            // need to look ahead to determine
            blockPhase = POST_VARS_PHASE;

            NonTerminal *newNode = new NonTerminal("");
            nextToken = consumeToken(false);
            if (nextToken->checkValue(Symbols::LPAREN)) {
                // must be a method call
                popFromTokens(); // let parseMethodCall get the next token
                newNode->modifyLabel("method_call");
                parseMethodCall(newNode);
            } else {
                // must be assignment 
                newNode->modifyLabel("assign_expr");
                parseAssignExpr(newNode);
            }

            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::SEMICOLON)) {
                throw ParsingError("Expected ';' at the end of a statement");
            }

            blockNode->addChild(newNode);
        } else if (nextToken->checkValue(Keywords::FOR)) {
            // this is a start of a for loop
            blockPhase = POST_VARS_PHASE;

            NonTerminal *forNode = new NonTerminal("for_node");
            parseFor(forNode);
            blockNode->addChild(forNode);
        } else if (nextToken->checkValue(Keywords::WHILE)) { 
            // this is a start of a while loop
            blockPhase = POST_VARS_PHASE;

            NonTerminal *whileNode = new NonTerminal("while_node");
            parseWhile(whileNode);
            blockNode->addChild(whileNode);
        } else if (nextToken->checkValue(Keywords::IF)) {
            // this is a start of an if-else sequence
            blockPhase = POST_VARS_PHASE;

            NonTerminal *ifNode = new NonTerminal("if_node");
            parseIfElse(ifNode);
            blockNode->addChild(ifNode);
        } else if (nextToken->checkValue(Keywords::RETURN)) {
            blockPhase = POST_VARS_PHASE;

            NonTerminal *returnNode = new NonTerminal("return_node");
            parseReturn(returnNode);
            blockNode->addChild(returnNode);
         } else if (nextToken->checkValue(Keywords::CONTINUE) || nextToken->checkValue(Keywords::BREAK)) {
            // these should be processed as needed
            blockPhase = POST_VARS_PHASE;

            blockNode->addChild(new Terminal(nextToken));
            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::SEMICOLON)) {
                throw ParsingError("Expected continue or break lines to end with a semicolon"); 
            }
        } else if (!nextToken->checkValue(Symbols::RBRACE)) {
            // unrecognized
            throw ParsingError("Expected a '}' at the end of a block");
        } 
        nextToken = consumeToken();
    }
}

void Parser::parseArrayIndex(NonTerminal *location) {
    NonTerminal *arrayIdxExpr = new NonTerminal("expr");
    parseExpr(arrayIdxExpr);
    location->addChild(arrayIdxExpr);

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RBRACKET)) {
        throw ParsingError("Array locations must end with ']'");
    }
}

void Parser::parseMethodCallOrAssignment(NonTerminal *methodCallOrStatement, Token *nextToken) {
    printDebug("Parsing method or statement");

    Token *prevToken = nextToken;
    nextToken = consumeToken(false);
    if (nextToken->checkValue(Symbols::LPAREN)) {
        // this must be a method call
        methodCallOrStatement->addChild(prevToken);
        methodCallOrStatement->modifyLabel("method_call");
        popFromTokens();
        parseMethodCall(methodCallOrStatement);
    } else {
        // otherwise it must be <location> <assign_expr> 
        methodCallOrStatement->modifyLabel("assign_expr");
        NonTerminal *location = new NonTerminal("location");
        location->addChild(prevToken);
        if (nextToken->checkValue(Symbols::LBRACKET)) {
            // the location is into an array
            location->addChild(nextToken);
            parseArrayIndex(location);
        }
        parseAssignExpr(methodCallOrStatement);
    }
} 

void Parser::parseAssignExpr(NonTerminal *assignExprNode) {
    printDebug("Parsing an assignment expression");

    Token *nextToken = consumeToken();
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        // the location is an array
        parseArrayIndex(assignExprNode);
        nextToken = consumeToken();
    }
    
    if (nextToken->isAssignOp()) {
        assignExprNode->addChild(nextToken);
        NonTerminal *expr = new NonTerminal("expr");
        parseExpr(expr);
        assignExprNode->addChild(expr);
    } else if (nextToken->isIncrementOp()) {
        assignExprNode->addChild(nextToken);
    } else {
        throw ParsingError("Expected either an incrementation or assing ops in assignment");
    } 
}

void Parser::parseMethodCall(NonTerminal *methodCallNode) {
    // syntax: <id> ([<expr>]*,)
    // must have parsed <id> into methodCallNode
    printDebug("Parsing a method call");

    Token *nextToken = consumeToken(false);
    while (!nextToken->checkValue(Symbols::RPAREN)) {
        NonTerminal *expr = new NonTerminal("expr");
        // expr->addChild(nextToken);
        parseExpr(expr);
        methodCallNode->addChild(expr);

        nextToken = consumeToken(false);
        if (nextToken->checkValue(Symbols::COMMA)) {
            //nextToken = consumeToken();
            popFromTokens();
            continue;
        } else if (!nextToken->checkValue(Symbols::RPAREN)) {
            throw ParsingError("Unexpected token " + string(*nextToken));
        }
    }
    popFromTokens();
    std::cout << "parseMethodcall returns" << std::endl;
}

void Parser::parseExpr(NonTerminal *expr) {
    printDebug("Parsing an expr");

    std::vector<NonTerminal*> terms;
    std::vector<Token*> ops;
    NonTerminal *term;
    Token *nextToken;
    while (true) {
        term = new NonTerminal("expr");
        parseTerm(term);
        terms.push_back(term);

        nextToken = consumeToken(false);
        if (nextToken->isBinaryOp()) {
            ops.push_back(nextToken);
            // delete from tokensQueue if nextToken is saved there
            popFromTokens(); 
        } else {
            break;
        }
    }

    parseExprHelper(expr, terms, ops);
    std::cout << "parseExpr returns" << std::endl;
}

void Parser::parseExprHelper(NonTerminal *expr, std::vector<NonTerminal*> terms, std::vector<Token*> ops) {
    std::cout << "parseExprPrime" << std::endl;
    NonTerminal *leftExpr = new NonTerminal("expr");
    NonTerminal *rightExpr = new NonTerminal("expr");
    if (ops.size() == 0) {
        expr->addChild(terms[0]);
    } else if (ops.size() == 1) {
        leftExpr->addChild(terms[0]);
        rightExpr->addChild(terms[1]);
        expr->addChild(leftExpr);
        expr->addChild(ops[0]);
        expr->addChild(rightExpr);
    } else if (ops[0]->isMorePrecedent(ops[1])) {
        leftExpr->addChild(terms[0]);
        leftExpr->addChild(ops[0]);
        leftExpr->addChild(terms[1]);
        parseExprHelper(rightExpr, std::vector<NonTerminal*>(terms.begin()+2, terms.end()), std::vector<Token*>(ops.begin()+2, ops.end()));
        expr->addChild(leftExpr);
        expr->addChild(ops[1]);
        expr->addChild(rightExpr);
    } else {
        leftExpr->addChild(terms[0]);
        parseExprHelper(rightExpr, std::vector<NonTerminal*>(terms.begin()+1, terms.end()), std::vector<Token*>(ops.begin()+1, ops.end()));
        expr->addChild(leftExpr);
        expr->addChild(ops[0]);
        expr->addChild(rightExpr);
    }
}

void Parser::parseTerm(NonTerminal *term) {
    printDebug("Parsing a term");

    Token *nextToken = consumeToken();
    NonTerminal *newNode = new NonTerminal("");
    if (nextToken->checkType(Token::IDENTIFIER)) {
        // location or method call
        newNode->addChild(nextToken);
        nextToken = consumeToken(false);
        if (nextToken->checkValue(Symbols::LPAREN)) {
            popFromTokens();
            newNode->modifyLabel("method_call"); 
            parseMethodCall(newNode);
        } else {
            newNode->modifyLabel("location");
            if (nextToken->checkValue(Symbols::LBRACKET)) {
                // array location
                popFromTokens();
                newNode->addChild(nextToken);
                parseArrayIndex(newNode);
            }
        }
    } else if (nextToken->isLiteral()) {
        newNode->modifyLabel("literal");
        newNode->addChild(nextToken);
    } else if (nextToken->checkValue(Keywords::LEN)) {
        newNode->modifyLabel("len");
        newNode->addChild(nextToken); 

        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::LPAREN)) {
            throw ParsingError("Len must be followed by (");
        }
        newNode->addChild(nextToken);

        nextToken = consumeToken();
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Expected an identifier after len(");
        }
        newNode->addChild(nextToken);

        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::RPAREN)) {
            throw ParsingError("Expected a ) after len(<id>"); 
        }
    } else if (nextToken->checkValue(Symbols::OP_MINUS) || nextToken->checkValue(Symbols::EXCALAMATION)) {
        term->addChild(nextToken);
        newNode->modifyLabel("expr");
        parseExpr(newNode);
    } else if (nextToken->checkValue(Symbols::LPAREN)) {
        // contrat the tree by parsing ((((expr)))) into just one expr node 
        parseExpr(term);
        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::RPAREN)) {
            throw ParsingError("Expected a matching ')' at the end");
        }
        // no need to reuse newNode, just return
        return;
    } else if (nextToken->checkType(Token::STRING_LITERAL)) {
        term->addChild(nextToken);
        newNode->modifyLabel("string_literal");
    } else {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    std::cout << "Returning term" << std::endl;
    term->addChild(newNode);
}

void Parser::parseReturn(NonTerminal *returnNode) {
    printDebug("Parsing a return statement");

    Token *nextToken = consumeToken(false);
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        // syntax: return <expr>
        NonTerminal *expr = new NonTerminal("expr");
        // parseExpr will see nextToken as it should be in tokens queue
        parseExpr(expr); 
        returnNode->addChild(expr);

        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::SEMICOLON)) {
            throw ParsingError("Expected a semicolon at the end of return");
        }
    }
}

void Parser::parseFor(NonTerminal *forNode) {
    printDebug("Parsing a for loop");

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Expected '(' after FOR");
    }
    
    NonTerminal *forInitiate = new NonTerminal("for_initiate");
    nextToken = consumeToken(); 
    if (!nextToken->checkType(Token::IDENTIFIER)) {
        throw ParsingError("Expected identifier but got " + string(*nextToken));
    }
    forInitiate->addChild(nextToken);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::EQUAL)) {
        throw ParsingError("Expected an '=' but got " + string(*nextToken));
    }
    forInitiate->addChild(nextToken);

    NonTerminal *expr = new NonTerminal("expr");
    parseExpr(expr);
    forInitiate->addChild(expr);
    
    forNode->addChild(forInitiate);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Expected a ';' but got " + string(*nextToken));
    }

    NonTerminal *forCondition = new NonTerminal("for_condition");
    expr  = new NonTerminal("expr");
    parseExpr(expr);
    forCondition->addChild(expr);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Expected a ';' but got " + string(*nextToken));
    }

    forNode->addChild(forCondition);

    NonTerminal *forUpdate = new NonTerminal("for_update");
    nextToken = consumeToken();
    if (!nextToken->checkType(Token::IDENTIFIER)) {
        throw ParsingError("Expected an identifier");
    }
    NonTerminal *var = new NonTerminal("location");
    var->addChild(nextToken);

    nextToken = consumeToken(false);
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        popFromTokens();
        var->addChild(nextToken);
        parseArrayIndex(var);
    }
    forUpdate->addChild(var);

    nextToken = consumeToken();
    if (nextToken->isCompoundAssignOp()) {
        forUpdate->addChild(nextToken);
        NonTerminal *updateExpr = new NonTerminal("expr");
        parseExpr(expr);
        forUpdate->addChild(updateExpr);
    } else if (nextToken->isIncrementOp()) {
        forUpdate->addChild(nextToken);
    } else {
        throw ParsingError("Inappropriate update for For loop");
    }

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    forNode->addChild(forUpdate);

    NonTerminal *forBlock = new NonTerminal("for_block");
    parseBlock(forBlock);
    
    forNode->addChild(forBlock);
}

void Parser::parseWhile(NonTerminal *whileNode) {
    printDebug("Parsing a while loop");

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    NonTerminal *whileUpdate = new NonTerminal("expr");
    parseExpr(whileUpdate);
    whileNode->addChild(whileUpdate);

    nextToken = consumeToken();
    if(!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    NonTerminal *whileBlock = new NonTerminal("while_block");
    parseBlock(whileBlock);
    whileNode->addChild(whileBlock);
}

void Parser::parseIfElse(NonTerminal *ifElseNode) {
    printDebug("Parsing an if-elif-else");

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    NonTerminal *ifCond = new NonTerminal("expr");
    parseExpr(ifCond);
    ifElseNode->addChild(ifCond); 

    nextToken = consumeToken();
    if(!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }

    NonTerminal *ifBlock = new NonTerminal("if_block");
    parseBlock(ifBlock);
    ifElseNode->addChild(ifBlock);

    nextToken = consumeToken();

    if (nextToken->checkValue(Keywords::ELIF)) {
        NonTerminal *elifNode = new NonTerminal("elif_node");
        parseIfElse(elifNode);
        ifElseNode->addChild(elifNode);
    } else if (nextToken->checkValue(Keywords::ELSE)) {
        NonTerminal *elseBlock = new NonTerminal("else_block");
        parseBlock(elseBlock);
    } else {
        tokensQueue.push(nextToken);
    }
}

Token *Parser::consumeToken(bool deleteToken) {
    Token *nextToken;
    if (tokensQueue.empty()) {
        nextToken = scanner->getNextToken();
        if (!deleteToken) {
            tokensQueue.push(nextToken);
        }
    } else {
        nextToken = tokensQueue.front();
        if (deleteToken) {
            tokensQueue.pop();
        }
    }

    printDebug("Read token " + string(*nextToken));

    // if (nextToken->checkType(Token::FILE_END)) {
    //     throw ParsingError("Unexpected EOF");
    // }

    return nextToken;
}

Token *Parser::consumeToken() {
    return consumeToken(true);
}

void Parser::popFromTokens() {
    if (!tokensQueue.empty()) {
        tokensQueue.pop();
    }
}

NonTerminal *Parser::program() {
    try {
        NonTerminal *imports = new NonTerminal("imports"); 
        NonTerminal *globalVars = new NonTerminal("global_vars");
        start->addChild(imports);
        globalVars->addChild(globalVars);
        Token *nextToken; 
        while (true) {
            nextToken = consumeToken();
            if (nextToken->checkValue(Keywords::IMPORT)) {
                if (phase != IMPORT_PHASE) {
                    throw ParsingError("Import statement after a non import statement is parsed");
                }
                NonTerminal *import = new NonTerminal("import");
                parseImport(import);
                imports->addChild(import);
                continue;
            }

            if (phase == IMPORT_PHASE) {
                phase = VAR_DECLRS_PHASE; 
            }

            if (nextToken->checkValue(Keywords::VOID)) {
                // this is a method
                phase = POST_VARS_PHASE;

                NonTerminal *method = new NonTerminal("method_decl"); 
                parseMethod(method, true);
                start->addChild(method);
            } else if (nextToken->isDataType()) {
                Token *dataType = nextToken;

                // this could be either a method or a global variable
                // look ahead to determine what 
                nextToken = consumeToken();
                if (!nextToken->checkType(Token::IDENTIFIER)) {
                    // identiifed needed to come right after data type 
                    throw ParsingError("Needed an identifier after a data type");
                }
                Token *identifier = nextToken;

                nextToken = consumeToken();
                if (nextToken->checkValue(Symbols::LPAREN)) {
                    // this means this production can only be a method one; 
                    phase = POST_VARS_PHASE;

                    NonTerminal *method = new NonTerminal("method_decl");
                    method->addChild(dataType);
                    method->addChild(identifier);
                    // method->addChild(nextToken);
                    parseMethod(method, false);
                    start->addChild(method);
                } else {
                    if (phase != VAR_DECLRS_PHASE) {
                        throw ParsingError("variables must be declared at the top of the block they are in");
                    } 

                    // otherwise it has to be a global variable
                    NonTerminal *var = new NonTerminal("var_decl");
                    var->addChild(dataType);
                    var->addChild(identifier);
                    // var->addChild(nextToken);
                    tokensQueue.push(nextToken);
                    parseVars(globalVars, var);    
                }       
            } else if (nextToken->checkType(Token::FILE_END)) {
                printDebug("EOF reached");
                return start;
            } else {
                printDebug("Unable to parse token " + string(*nextToken));
                throw ParsingError("Cannot parse token "  + string(*nextToken));
            }
        }
 
        return start;
    } catch (Exception &e) {
        throw e;
    }
}