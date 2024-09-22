cc = g++
h = ./headerFiles/
s = ./src/
flags = #-Wall -Werror
lib = ./lib/

all: main

main: $(s)inputTraversal.cpp $(h)mazeFunctions.h $(h)mazeTraverser.h $(h)randomNum.h
	$(cc) $(flags) $(s)inputTraversal.cpp $(lib)libraylib.a

