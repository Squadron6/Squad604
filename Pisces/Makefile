# Makefile for Static and Dynamic Analysis of Fish
all: pisces

pisces: main.o GraphMaker.o Colourizer.o StaticReader.o DynamicReader.o CodeInjector.o
	g++ main.o GraphMaker.o Colourizer.o StaticReader.o DynamicReader.o CodeInjector.o -o pisces

main.o: main.cpp
	g++ -c -std=c++11 main.cpp

GraphMaker.o: GraphMaker.cpp
	g++ -c -std=c++11 GraphMaker.cpp

Colourizer.o: Colourizer.cpp
	g++ -c -std=c++11 Colourizer.cpp

StaticReader.o: StaticReader.cpp
	g++ -c -std=c++11 StaticReader.cpp

DynamicReader.o: DynamicReader.cpp
	g++ -c -std=c++11 DynamicReader.cpp

CodeInjector.o: CodeInjector.cpp
	g++ -c -std=c++11 CodeInjector.cpp

clean:
	rm -rf *o pisces
