##################################################
# Author: Simon Hofmann <mail@simon-hofmann.org>
# Date:   2014-08-13
##################################################

FLAGS    += `pkg-config --libs --cflags opencv` -std=c++11

Eval: main.o evaluate.o exception.o
	g++ ${FLAGS} -g main.o evaluate.o exception.o -o Evaluator

main.o: main.cpp
	g++ -g -c main.cpp

evaluate.o: evaluate.cpp
	g++ -g -c evaluate.cpp

exception.o: exception.cpp
	g++ -g -c exception.cpp
