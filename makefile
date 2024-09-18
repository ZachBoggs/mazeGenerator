cc = g++
h = ./headerFiles/
flags = #-Wall -Werror
lib = ./lib/

all: main


textureTest: spriteReader.cpp
	$(cc) $(flags) spriteReader.cpp $(lib)libraylib.a

mazePieces: mazePieceViewer.cpp
	$(cc) $(flags) mazePieceViewer.cpp $(lib)libraylib.a

sample: sampleMaze.cpp mazeFunctions.h
	$(cc) $(flags) sampleMaze.cpp $(lib)libraylib.a

main: inputTraversal.cpp $(h)mazeFunctions.h $(h)mazeTraverser.h $(h)randomNum.h
	$(cc) $(flags) inputTraversal.cpp $(lib)libraylib.a

