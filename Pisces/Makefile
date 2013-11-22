# Makefile for Dynamic Analysis
all: dynamic

dynamic: PiscesDynamic.o DynamicReader.o CodeInjector.o
	g++ PiscesDynamic.o DynamicReader.o CodeInjector.o -o dynamic

PiscesDynamic.o: PiscesDynamic.cpp
	g++ -c -std=c++11 PiscesDynamic.cpp

DynamicReader.o: DynamicReader.cpp
	g++ -c -std=c++11 DynamicReader.cpp

CodeInjector.o: CodeInjector.cpp
	g++ -c -std=c++11 CodeInjector.cpp

clean:
	rm -rf *o dynamic
