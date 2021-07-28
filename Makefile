# we use the g++ compiler 
CC = g++ -std=c++14

# compiler flags:
#  -g : add debugging info to the executable 
#  -Wall : turm on compiler warnings 
FLAGS = -g -Wall

MAIN_DEPS = ./build/main.o ./build/cli.o ./build/common.o ./build/exceptions.o ./build/scanner.o ./build/token.o ./build/parser.o ./build/parsetree.o\

SCANNER_CONSTS = ./src/scanner/scanner.h ./src/commons/keywords.h ./src/commons/defs.h ./src/commons/symbols.h ./src/scanner/token.h
SCANNER_DEPS = ./build/exceptions.o ./build/token.o 

DEFS_H = ./src/commons/defs.h 

TOKEN_CONSTS = ./src/scanner/token.h ./src/commons/defs.h 

AST_HEADERS = ./src/ast/assignnode.h ./src/ast/astnode.h ./src/ast/binaryexprnode.h ./src/ast/blocknode.h ./src/ast/breakcontinuenode.h ./src/ast/exprnode.h ./src/ast/fornode.h ./src/ast/ifelsenode.h \
			  ./src/ast/importnode.h ./src/ast/lennode.h ./src/ast/literalnode.h ./src/ast/locnode.h ./src/ast/methodargnode.h ./src/ast/methoddeclnode.h ./src/ast/programnode.h\
			  ./src/ast/returnnode.h ./src/ast/unaryexprnode.h ./src/ast/vardeclnode.h ./src/ast/whilenode.h

PARSER_CONSTS = ./src/parser/parser.h ./src/parser/parsetree.h $(SCANNER_CONSTS) $(AST_HEADERS)
PARSER_DEPS = ./build/parsetree.o ./build/scanner.o ./build/exceptions.o ./build/token.o ./build/ast.o

TOKEN_O = ./build/token.o 
COMMON_O = ./build/common.o 
EXCEPTIONS_O = ./build/exceptions.o 

TOKEN_H = ./src/scanner/token.h
COMMON_H = ./src/commons/common.h
KEYWORDS_H = ./src/commons/keywords.h 
SYMBOLS_H = ./src/commons/symbols.h 

ASSIGN_O = ./build/assignnode.o 
AST_O = ./build/ast.o 
BINARY_EXPR_O = ./build/binaryexprnode.o 
BLOCK_O = ./build/blocknode.o 
BREAK_CONTINUE_O = ./build/breakcontinuenode.o 
EXPR_O = ./build/exprnode.o 
FOR_O = ./build/fornode.o 
IF_ELSE_O = ./build/ifelsenode.o 
IMPORT_O = ./build/importnode.o 
LEN_O = ./build/lennode.o 
LITERAL_O = ./build/literalnode.o 
LOC_O = ./build/locnode.o 
METHOD_CALL_O = ./build/methodcallnode.o 
METHOD_DECL_O = ./build/methoddeclnode.o 
PROGRAM_O = ./build/programnode.o 
RETURN_O = ./build/returnnode.o 
UNARY_EXPR_O = ./build/unaryexprnode.o 
VAR_DECL_O = ./build/vardeclnode.o 
WHILE_O = ./build/whilenode.o 

AST_OS = $(ASSIGN_O) $(AST_O) $(BINARY_EXPR_O) $(BLOCK_O) $(BREAK_CONTINUE_O) $(EXPR_O) $(FOR_O) $(IF_ELSE_O) $(IMPORT_O) $(LEN_O) $(LITERAL_O) $(LOC_O) $(METHOD_CALL_O) \
		 $(METHOD_DECL_O) $(PROGRAM_O) $(RETURN_O) $(UNARY_EXPR_O) $(VAR_DECL_O) $(WHILE_O)

MAIN_DEPS = ./build/main.o ./build/cli.o ./build/common.o ./build/exceptions.o ./build/scanner.o ./build/token.o ./build/parser.o ./build/parsetree.o $(AST_OS)\

ASSIGN_H = ./src/ast/assignnode.h 
ASSIGN_C = ./src/ast/assignnode.cc 
AST_H = ./src/ast/astnode.h 
AST_C = ./src/ast/astnode.cc 
BINARY_EXPR_H = ./src/ast/binaryexprnode.h 
BINARY_EXPR_C = ./src/ast/binaryexprnode.cc 
BLOCK_H = ./src/ast/blocknode.h 
BLOCK_C = ./src/ast/blocknode.cc 
BREAK_CONTINUE_H = ./src/ast/breakcontinuenode.h 
BREAK_CONTINUE_C = ./src/ast/breakcontinuenode.cc 
EXPR_H = ./src/ast/exprnode.h 
EXPR_C = ./src/ast/exprnode.cc 
FOR_H = ./src/ast/fornode.h 
FOR_C = ./src/ast/fornode.cc 
IF_ELSE_H = ./src/ast/ifelsenode.h 
IF_ELSE_C = ./src/ast/ifelsenode.cc 
IMPORT_H = ./src/ast/importnode.h 
IMPORT_C = ./src/ast/importnode.cc 
LEN_H = ./src/ast/lennode.h 
LEN_C = ./src/ast/lennode.cc 
LITERAL_H = ./src/ast/literalnode.h 
LITERAL_C = ./src/ast/literalnode.cc 
LOC_H = ./src/ast/locnode.h 
LOC_C = ./src/ast/locnode.cc 
METHOD_ARG_H = ./src/ast/methodargnode.h
METHOD_CALL_H = ./src/ast/methodcallnode.h 
METHOD_CALL_C = ./src/ast/methodcallnode.cc 
METHOD_DECL_H = ./src/ast/methoddeclnode.h 
METHOD_DECL_C = ./src/ast/methoddeclnode.cc 
PROGRAM_H = ./src/ast/programnode.h 
PROGRAM_C = ./src/ast/programnode.cc 
RETURN_H = ./src/ast/returnnode.h 
RETURN_C = ./src/ast/returnnode.cc 
UNARY_EXPR_H = ./src/ast/unaryexprnode.h 
UNARY_EXPR_C = ./src/ast/unaryexprnode.cc 
VAR_DECL_H = ./src/ast/vardeclnode.h 
VAR_DECL_C = ./src/ast/vardeclnode.cc 
WHILE_H = ./src/ast/whilenode.h 
WHILE_C = ./src/ast/whilenode.cc  


main: $(MAIN_DEPS)
	$(CC) $(FLAGS) $(MAIN_DEPS) -o main

./build/main.o: ./src/main/main.cc ./src/commons/defs.h ./build/cli.o ./build/common.o ./build/exceptions.o
	$(CC) $(FLAGS) -c ./src/main/main.cc ./build/cli.o ./build/common.o ./build/exceptions.o -o ./build/main.o 

./build/cli.o : ./src/util/cli.h ./src/util/cli.cc ./src/commons/defs.h ./build/common.o ./build/exceptions.o
	$(CC) $(FLAGS) -c ./src/util/cli.cc ./build/common.o ./build/exceptions.o -o ./build/cli.o

./build/exceptions.o : ./src/commons/exceptions.h ./src/commons/exceptions.cc ./src/commons/defs.h ./build/common.o
	$(CC) $(FLAGS) -c ./src/commons/exceptions.cc ./build/common.o -o ./build/exceptions.o

./build/common.o: ./src/commons/common.h ./src/commons/common.cc ./src/commons/defs.h
	$(CC) $(FLAGS) -c ./src/commons/common.cc -o ./build/common.o

./build/scanner.o: $(SCANNER_CONSTS) ./src/scanner/scanner.cc $(SCANNER_DEPS)
	$(CC) $(FLAGS) -c ./src/scanner/scanner.cc -o ./build/scanner.o 

./build/token.o: $(TOKEN_CONSTS) ./src/scanner/token.cc ./build/exceptions.o 
	$(CC) $(FLAGS) -c ./src/scanner/token.cc -o ./build/token.o 

./build/parsetree.o: ./src/parser/parsetree.h ./src/parser/parsetree.cc ./src/commons/defs.h ./src/scanner/token.h
	$(CC) $(FLAGS) -c ./src/parser/parsetree.cc -o ./build/parsetree.o 

./build/parser.o: $(PARSER_CONSTS) ./src/parser/parser.cc  $(PARSER_DEPS) $(AST_HEADERS) $(AST_OS)
	$(CC) $(FLAGS) -c ./src/parser/parser.cc -o ./build/parser.o

$(ASSIGN_O): $(ASSIGN_H) $(ASSIGN_C) $(DEFS_H) $(TOKEN_O) $(LOC_O) $(EXPR_O) $(COMMON_O)
	$(CC) $(FLAGS) -c $(ASSIGN_C) -o $(ASSIGN_O) 

$(AST_O): $(AST_H) $(AST_H) $(DEFS_H) $(TOKEN_O)
	$(CC) $(FLAGS) -c $(AST_C) -o $(AST_O)

$(BINARY_EXPR_O): $(BINARY_EXPR_H) $(BINARY_EXPR_C) $(DEFS_H) $(TOKEN_O) $(EXPR_O) $(COMMON_O)
	$(CC) $(FLAGS) -c $(BINARY_EXPR_C) -o $(BINARY_EXPR_O) 

$(BLOCK_O): $(BLOCK_H) $(BLOCK_C) $(DEFS_H) $(VAR_DECL_O) $(AST_O) $(COMMON_O)
	$(CC) $(FLAGS) -c $(BLOCK_C) -o $(BLOCK_O)

$(BREAK_CONTINUE_O): $(BREAK_CONTINUE_H) $(BREAK_CONTINUE_C) $(DEFS_H) $(TOKEN_O) $(KEYWORDS_H) 
	$(CC) $(FLAGS) -c $(BREAK_CONTINUE_C) -o $(BREAK_CONTINUE_O)

$(EXPR_O): $(EXPR_H) $(EXPR_C) $(DEFS_H) $(AST_H) $(AST_O) $(TOKEN_O) $(EXCEPTIONS_O) $(COMMON_O)
	$(CC) $(FLAGS) -c $(EXPR_C) -o $(EXPR_O)

$(FOR_O): $(FOR_H) $(FOR_C) $(DEFS_H) $(AST_O) $(ASSIGN_O) $(EXPR_O) $(BLOCK_O) $(COMMON_O) 
	$(CC) $(FLAGS) -c $(FOR_C) -o $(FOR_O)

$(IF_ELSE_O): $(IF_ELSE_H) $(IF_ELSE_C) $(AST_O) $(EXPR_O) $(BLOCK_O) $(DEFS) $(COMMON_O)
	$(CC) $(FLAGS) -c $(IF_ELSE_C) -o $(IF_ELSE_O) 

$(IMPORT_O): $(IMPORT_H) $(IMPORT_C) $(AST_O) $(LITERAL_O) $(DEFS_H) $(COMMON_O) 
	$(CC) $(FLAGS) -c $(IMPORT_C) -o $(IMPORT_O)

$(LEN_O): $(LEN_H) $(LEN_C) $(EXPR_O) $(LITERAL_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(LEN_C) -o $(LEN_O)

$(LITERAL_O): $(LITERAL_H) $(LITERAL_C) $(AST_O) $(EXPR_O) $(DEFS_H) $(TOKEN_O) $(COMMON_O)
	$(CC) $(FLAGS) -c $(LITERAL_C) -o $(LITERAL_O)

$(LOC_O): $(LOC_H) $(LOC_C) $(LITERAL_O) $(EXPR_O) $(TOKEN_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(LOC_C) -o $(LOC_O)

$(METHOD_CALL_O): $(METHOD_CALL_H) $(METHOD_CALL_C) $(EXPR_O) $(LITERAL_O) $(AST_O) $(TOKEN_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(METHOD_CALL_C) -o $(METHOD_CALL_O)

$(METHOD_DECL_O): $(METHOD_DECL_H) $(METHOD_DECL_C) $(AST_O) $(LITERAL_O) $(BLOCK_O) $(METHOD_ARG_H) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(METHOD_DECL_C) -o $(METHOD_DECL_O) 

$(PROGRAM_O): $(PROGRAM_H) $(PROGRAM_C) $(AST_O) $(IMPORT_O) $(VAR_DECL_O) $(METHOD_DECL_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(PROGRAM_C) -o $(PROGRAM_O)

$(RETURN_O): $(RETURN_H) $(RETURN_C) $(AST_O) $(EXPR_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(RETURN_C) -o $(RETURN_O)

$(UNARY_EXPR_O): $(UNARY_EXPR_H) $(UNARY_EXPR_C) $(EXPR_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(UNARY_EXPR_C) -o $(UNARY_EXPR_O)

$(VAR_DECL_O): $(VAR_DECL_H) $(VAR_DECL_C) $(AST_H) $(AST_O) $(LITERAL_H) $(LITERAL_O) $(DEFS_H) $(TOKEN_H) $(TOKEN_O) $(COMMON_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(VAR_DECL_C) -o $(VAR_DECL_O)

$(WHILE_O): $(WHILE_H) $(WHILE_C) $(AST_O) $(EXPR_O) $(BLOCK_O) $(DEFS_H) $(COMMON_O)
	$(CC) $(FLAGS) -c $(WHILE_C) -o $(WHILE_O)

clean:
	rm ./build/*.o main

