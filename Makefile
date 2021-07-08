# we use the g++ compiler 
CC = g++ -std=c++17

# compiler flags:
#  -g : add debugging info to the executable 
#  -Wall : turm on compiler warnings 
FLAGS = -g -Wall

MAIN_DEPS = ./build/main.o ./build/cli.o ./build/common.o ./build/exceptions.o

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

clean:
	rm ./build/*.o main

