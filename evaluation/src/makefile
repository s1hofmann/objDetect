##################################################
# Author: Simon Hofmann <mail@simon-hofmann.org>
# Date:   2014-08-13
##################################################

FLAGS    += `pkg-config --libs --cflags opencv`
CFLAGS   += -g -std=c++11
CC 	     = g++

Eval: main.o evaluate.o exception.o
	${CC} ${FLAGS} ${CFLAGS} main.o evaluate.o exception.o -o Evaluator

main.o: main.cpp
	${CC} ${CFLAGS} -c main.cpp

evaluate.o: evaluate.cpp
	${CC} ${CFLAGS} -c evaluate.cpp

exception.o: exception.cpp
	${CC} ${CFLAGS} -c exception.cpp

clean:
	@rm *.o
	@rm Evaluator
