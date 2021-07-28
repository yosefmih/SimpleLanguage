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
#include "../commons/common.h"

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

void Parser::parseImport(ImportNode *import) {
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
    import->setIdentifier(new LitNode(nextToken));

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Invaid import at " + scanner->getLocationAsStr() + ". All imports must end with ;");
    }
}


void Parser::parseArrayDecl(VarDeclNode *var) {
    printDebug("Parsing an array decl");

    Token *nextToken = consumeToken();
    if (!nextToken->checkType(Token::INT_LITERAL)) {
        // there must be an int in between the brackets
        throw ParsingError("Array vars must have an int in between the brackets");
    }
    var->setSize(stringToInt(nextToken->value));

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RBRACKET)) {
        throw ParsingError("Array vars must end with a closng bracket");
    }
}


void Parser::parseVars(ASTNode *varsContainer, VarDeclNode *var) {
    printDebug("Parsing variable declarations");

    Token *nextToken = consumeToken();
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        // this is an array
        // <data type> <id>[<int>]
        parseArrayDecl(var);
        nextToken = consumeToken();
    }
    varsContainer->addVar(var);

    VarDeclNode *newVar;
    // parse more linked up vars
    while (nextToken->checkValue(Symbols::COMMA)) {
        newVar = new VarDeclNode();
        nextToken = consumeToken();
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Chained variable declaration must have identifier");
        }
        newVar->setIdentifier(new LitNode(nextToken));

        nextToken = consumeToken();
        if (nextToken->checkValue(Symbols::LBRACKET)) {
            parseArrayDecl(newVar);
            nextToken = consumeToken();
        }

        varsContainer->addVar(newVar);
    }

    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Unexpected token " + string(*nextToken) + " while parsing variable declarations");
    }
}

void Parser::parseMethod(MethodDeclNode *method, bool voidMethod) {
    printDebug("Parsing a method declaration");

    Token *nextToken;
    if (voidMethod) {
        nextToken = consumeToken();
        // need to fetch id and lparen before the non void methods and this are equal
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Type must be followed by identifier for method declaration"); 
        }
        method->setIdentifier(new LitNode(nextToken));

        // expected to be '(' 
        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::LPAREN)) {
            throw ParsingError("Expected '(' after identifier in method declaration");
        }
    }

    // parse the method args
    nextToken = consumeToken();
    MethodArgNode *arg;
    while (!nextToken->checkValue(Symbols::RPAREN)) {
        arg = new MethodArgNode();
        if (!nextToken->isDataType()) {
            throw ParsingError("Expected a type in arguments");
        }
        arg->setType(nextToken);

        nextToken = consumeToken();
        if (!nextToken->checkType(Token::IDENTIFIER)) {
            throw ParsingError("Expected an identifier in arguments");
        }
        arg->setIdentifier(new LitNode(nextToken));

        method->addArg(arg);

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

    BlockNode *methodBlock = parseBlock();
    method->setBlock(methodBlock);
}

BlockNode *Parser::parseBlock() {
    printDebug("Parsing a block");

    uint32_t blockPhase = VAR_DECLRS_PHASE;
    BlockNode *blockNode = new BlockNode();
    Token *nextToken = consumeToken();
    // must start with '{'
    if (!nextToken->checkValue(Symbols::LBRACE)) {
        throw ParsingError("Blocks must start with '{'");
    } 

    nextToken = consumeToken();
    while (!nextToken->checkValue(Symbols::RBRACE)) {
        if (nextToken->isDataType()) {
            VarDeclNode *var = new VarDeclNode();
            // this has to be a variable declaration
            if (blockPhase != VAR_DECLRS_PHASE) {
                throw ParsingError("all variable declaration must be at the top of a block");
            }
            var->setType(nextToken);
            nextToken = consumeToken();
            if (!nextToken->checkType(Token::IDENTIFIER)) {
                throw ParsingError("Identifier must come after type in variable declaration");
            }
            var->setIdentifier(new LitNode(nextToken));
            parseVars(blockNode, var); 
        } else if (nextToken->checkType(Token::IDENTIFIER)) {
            // either  <location> <assign_expr>; or <method_call>;
            // need to look ahead to determine
            blockPhase = POST_VARS_PHASE;
            ASTNode *newNode;
            LitNode *identifier = new LitNode(nextToken);
            nextToken = consumeToken(false);
            if (nextToken->checkValue(Symbols::LPAREN)) {
                // must be a method call
                popFromTokens(); // let parseMethodCall get the next token
                newNode = parseMethodCall(identifier);
            } else {
                // must be assignment 
                newNode = parseAssignExpr(new LocNode(identifier));
            }
            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::SEMICOLON)) {
                throw ParsingError("Expected ';' at the end of a statement");
            }
            blockNode->addStatement(newNode);
        } else if (nextToken->checkValue(Keywords::FOR)) {
            // this is a start of a for loop
            blockPhase = POST_VARS_PHASE;
            ForNode *forNode = parseFor();
            blockNode->addStatement(forNode);
        } else if (nextToken->checkValue(Keywords::WHILE)) { 
            // this is a start of a while loop
            blockPhase = POST_VARS_PHASE;
            WhileNode *whileNode = parseWhile();
            blockNode->addStatement(whileNode);
        } else if (nextToken->checkValue(Keywords::IF)) {
            // this is a start of an if-else sequence
            blockPhase = POST_VARS_PHASE;
            IfElseNode *ifNode = parseIfElse();
            blockNode->addStatement(ifNode);
        } else if (nextToken->checkValue(Keywords::RETURN)) {
            blockPhase = POST_VARS_PHASE;
            ReturnNode *returnNode = parseReturn();
            blockNode->addStatement(returnNode);
         } else if (nextToken->checkValue(Keywords::CONTINUE) || nextToken->checkValue(Keywords::BREAK)) {
            // these should be processed as needed
            blockPhase = POST_VARS_PHASE;
            blockNode->addStatement(new BreakContinueNode(nextToken));
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

LocNode *Parser::parseArrayIndex(LitNode *identifier) {
    LocNode *loc = new LocNode(identifier);
    ExprNode *arrayIdxExpr = parseExpr();
    loc->setIndex(arrayIdxExpr);

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RBRACKET)) {
        throw ParsingError("Array locations must end with ']'");
    }

    return loc;
}

AssignNode *Parser::parseAssignExpr(LocNode *loc) {
    printDebug("Parsing an assignment expression");
    AssignNode *assignNode = new AssignNode();
    Token *nextToken = consumeToken();
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        // the location is an array
        loc = parseArrayIndex(loc->getIdentifier());
        nextToken = consumeToken();
    }
    assignNode->setLocation(loc);
    
    if (nextToken->isAssignOp()) {
        assignNode->setAssignmentOp(nextToken);
        ExprNode *expr = parseExpr();
        assignNode->setExpr(expr);
    } else if (nextToken->isIncrementOp()) {
        assignNode->setAssignmentOp(nextToken);
    } else {
        throw ParsingError("Expected either an incrementation or assing ops in assignment");
    } 
}

MethodCallNode *Parser::parseMethodCall(LitNode *identifier) {
    // syntax: <id> ([<expr>]*,)
    // must have parsed <id> into methodCallNode
    printDebug("Parsing a method call");
    MethodCallNode *methodCallNode = new MethodCallNode();
    ExprNode *expr;
    Token *nextToken = consumeToken(false);
    while (!nextToken->checkValue(Symbols::RPAREN)) {
        expr = parseExpr();
        methodCallNode->addArg(expr);
        nextToken = consumeToken(false);
        if (nextToken->checkValue(Symbols::COMMA)) {
            popFromTokens();
            continue;
        } else if (!nextToken->checkValue(Symbols::RPAREN)) {
            throw ParsingError("Unexpected token " + string(*nextToken));
        }
    }
    popFromTokens();
}

ExprNode *Parser::parseExpr() {
    printDebug("Parsing an expr");
    std::vector<ExprNode*> terms;
    std::vector<Token*> ops;
    ExprNode *term;
    Token *nextToken;
    while (true) {
        nextToken = consumeToken();
        if (nextToken->checkType(Token::IDENTIFIER)) {
            // location or method call
            LitNode *identifier = new LitNode(nextToken);
            nextToken = consumeToken(false);
            if (nextToken->checkValue(Symbols::LPAREN)) {
                popFromTokens();
                // ((MethodCallNode*)term)->setIdentifier(identifier);
                term = parseMethodCall(identifier);
            } else {
                term = new LocNode(identifier);
                if (nextToken->checkValue(Symbols::LBRACKET)) {
                    // array location
                    popFromTokens();
                    term = parseArrayIndex(identifier);
                }
            }
        } else if (nextToken->isLiteral() || nextToken->checkType(Token::STRING_LITERAL)) {
            term = new LitNode(nextToken);
            // ((LitNode*)term)->setToken(nextToken);
        } else if (nextToken->checkValue(Keywords::LEN)) {
            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::LPAREN)) {
                throw ParsingError("Len must be followed by (");
            }

            nextToken = consumeToken();
            if (!nextToken->checkType(Token::IDENTIFIER)) {
                throw ParsingError("Expected an identifier after len(");
            }

            term = new LenNode(new LitNode(nextToken));

            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::RPAREN)) {
                throw ParsingError("Expected a ) after len(<id>"); 
            }
        } else if (nextToken->checkValue(Symbols::OP_MINUS) || nextToken->checkValue(Symbols::EXCALAMATION)) {
            term = new UnaryExprNode();
            ((UnaryExprNode*)term)->setOp(nextToken);
            ExprNode *child = parseExpr();
            ((UnaryExprNode*)term)->setChild(child);
        } else if (nextToken->checkValue(Symbols::LPAREN)) {
            // contrat the tree by parsing ((((expr)))) into just one expr node 
            term = parseExpr();
            nextToken = consumeToken();
            if (!nextToken->checkValue(Symbols::RPAREN)) {
                throw ParsingError("Expected a matching ')' at the end");
            }
        } else {
            throw ParsingError("Unexpected token " + string(*nextToken));
        }
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

    return parseExprHelper(terms, ops);
}

ExprNode *Parser::parseExprHelper(std::vector<ExprNode*> terms, std::vector<Token*> ops) {
    ExprNode *leftExpr;
    ExprNode *rightExpr;
    if (ops.size() == 0) {
        return terms[0];
    } else if (ops.size() == 1) {
        return new BinaryExprNode(ops[0], terms[0], terms[1]);
    } else if (ops[0]->isMorePrecedent(ops[1])) {
        leftExpr = new BinaryExprNode(ops[0], terms[0], terms[1]);
        rightExpr = parseExprHelper(std::vector<ExprNode*>(terms.begin()+2, terms.end()), std::vector<Token*>(ops.begin()+2, ops.end()));
        return new BinaryExprNode(ops[1], leftExpr, rightExpr);
    } else {
        rightExpr = parseExprHelper(std::vector<ExprNode*>(terms.begin()+1, terms.end()), std::vector<Token*>(ops.begin()+1, ops.end()));
        return new BinaryExprNode(ops[0], terms[0], rightExpr);
    }
}


ReturnNode *Parser::parseReturn() {
    printDebug("Parsing a return statement");
    ReturnNode *returnNode = new ReturnNode();
    Token *nextToken = consumeToken(false);
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        // syntax: return <expr>
        ExprNode *expr = parseExpr(); 
        returnNode->setExpr(expr);
        nextToken = consumeToken();
        if (!nextToken->checkValue(Symbols::SEMICOLON)) {
            throw ParsingError("Expected a semicolon at the end of return");
        }
    }
    return returnNode;
}

ForNode *Parser::parseFor() {
    printDebug("Parsing a for loop");
    ForNode *forNode = new ForNode();

    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Expected '(' after FOR");
    } 
    AssignNode *forInitiate = new AssignNode();
    nextToken = consumeToken(); 
    if (!nextToken->checkType(Token::IDENTIFIER)) {
        throw ParsingError("Expected identifier but got " + string(*nextToken));
    }
    forInitiate->setLocation(new LocNode(new LitNode(nextToken)));
    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::EQUAL)) {
        throw ParsingError("Expected an '=' but got " + string(*nextToken));
    }
    forInitiate->setAssignmentOp(nextToken);
    ExprNode *expr = parseExpr();
    forInitiate->setExpr(expr);
    forNode->setInitiate(forInitiate);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Expected a ';' but got " + string(*nextToken));
    }
    ExprNode *forCondition = parseExpr();
    forNode->setCond(forCondition);

    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::SEMICOLON)) {
        throw ParsingError("Expected a ';' but got " + string(*nextToken));
    }
    AssignNode *forUpdate = new AssignNode();
    nextToken = consumeToken();
    if (!nextToken->checkType(Token::IDENTIFIER)) {
        throw ParsingError("Expected an identifier");
    }
    LocNode *loc = new LocNode(new LitNode(nextToken));
    nextToken = consumeToken(false);
    if (nextToken->checkValue(Symbols::LBRACKET)) {
        popFromTokens();
        loc = parseArrayIndex(loc->getIdentifier());
    }
    forUpdate->setLocation(loc);
    nextToken = consumeToken();
    if (nextToken->isCompoundAssignOp()) {
        forUpdate->setAssignmentOp(nextToken);
        ExprNode *updateExpr = parseExpr();
        forUpdate->setExpr(updateExpr);
    } else if (nextToken->isIncrementOp()) {
        forUpdate->setAssignmentOp(nextToken);
    } else {
        throw ParsingError("Inappropriate update for For loop");
    }
    nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }
    forNode->setUpdate(forUpdate);

    BlockNode *forBlock = parseBlock();
    forNode->setBlock(forBlock);
    
    return forNode;
}

WhileNode *Parser::parseWhile() {
    printDebug("Parsing a while loop");
    WhileNode *whileNode = new WhileNode();
    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }
    ExprNode *whileCond = parseExpr();
    whileNode->setCond(whileCond);

    nextToken = consumeToken();
    if(!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }
    BlockNode *whileBlock =  parseBlock();
    whileNode->setBlock(whileBlock);

    return whileNode;
}

IfElseNode *Parser::parseIfElse() {
    printDebug("Parsing an if-elif-else");
    IfElseNode *ifElseNode = new IfElseNode();
    Token *nextToken = consumeToken();
    if (!nextToken->checkValue(Symbols::LPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }
    ExprNode *ifCond = parseExpr();
    ifElseNode->setCond(ifCond); 

    nextToken = consumeToken();
    if(!nextToken->checkValue(Symbols::RPAREN)) {
        throw ParsingError("Unexpected token " + string(*nextToken));
    }
    BlockNode *ifBlock = parseBlock();
    ifElseNode->setBlock(ifBlock);

    nextToken = consumeToken();
    if (nextToken->checkValue(Keywords::ELIF)) {
        IfElseNode *elifNode = parseIfElse();
        ifElseNode->setElifNode(elifNode);
    } else if (nextToken->checkValue(Keywords::ELSE)) {
        BlockNode *elseBlock = parseBlock();
        ifElseNode->setElseBlock(elseBlock);
    } else {
        tokensQueue.push(nextToken);
    }

    return ifElseNode;
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

ProgramNode *Parser::program() {
    try {
        ProgramNode *program = new ProgramNode();
        NonTerminal *imports = new NonTerminal("imports"); 
        NonTerminal *globalVars = new NonTerminal("global_vars");
        start->addChild(imports);
        globalVars->addChild(globalVars);
        Token *nextToken; 
        ImportNode *import;
        VarDeclNode *var;
        MethodDeclNode *method;
        while (true) {
            nextToken = consumeToken();
            if (nextToken->checkValue(Keywords::IMPORT)) {
                if (phase != IMPORT_PHASE) {
                    throw ParsingError("Import statement after a non import statement is parsed");
                }
                import = new ImportNode();
                parseImport(import);
                program->addImport(import);
                continue;
            }

            if (phase == IMPORT_PHASE) {
                phase = VAR_DECLRS_PHASE; 
            }

            if (nextToken->checkValue(Keywords::VOID)) {
                // this is a method
                phase = POST_VARS_PHASE;

                method = new MethodDeclNode(); 
                parseMethod(method, true);
                program->addMethod(method);
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

                nextToken = consumeToken();;
                if (nextToken->checkValue(Symbols::LPAREN)) {
                    // this means this production can only be a method one; 
                    phase = POST_VARS_PHASE;

                    method = new MethodDeclNode(dataType, new LitNode(identifier));
                    // method->addChild(nextToken);
                    parseMethod(method, false);
                    program->addMethod(method);
                } else {
                    if (phase != VAR_DECLRS_PHASE) {
                        throw ParsingError("variables must be declared at the top of the block they are in");
                    } 

                    // otherwise it has to be a global variable
                    var = new VarDeclNode(new LitNode(identifier), dataType);
                    program->addVar(var);
                    tokensQueue.push(nextToken);
                    parseVars(program, var);   
                }       
            } else if (nextToken->checkType(Token::FILE_END)) {
                printDebug("EOF reached");
                return program;
            } else {
                printDebug("Unable to parse token " + string(*nextToken));
                throw ParsingError("Cannot parse token "  + string(*nextToken));
            }
        }
 
        return program;
    } catch (Exception &e) {
        throw e;
    }
}