##################################################
# Author: Simon Hofmann <mail@simon-hofmann.org>
# Date:   2014-08-13
##################################################

NAME 	 = opencv_evaluate
FLAGS    += `pkg-config --libs --cflags opencv`
CFLAGS   += -g -std=c++11
CC 	     = g++

Eval: ./bin/main.o ./bin/evaluate.o ./bin/exception.o
	${CC} ${FLAGS} ${CFLAGS} ./bin/main.o ./bin/evaluate.o ./bin/exception.o -o opencv_evaluate

bin/main.o: ./src/main.cpp
	${CC} ${CFLAGS} -c ./src/main.cpp -o ./bin/main.o

bin/evaluate.o: ./src/evaluate.cpp
	${CC} ${CFLAGS} -c ./src/evaluate.cpp -o ./bin/evaluate.o

bin/exception.o: ./src/exception.cpp
	${CC} ${CFLAGS} -c ./src/exception.cpp -o ./bin/exception.o

clean:
	@rm ./bin/*.o
	@rm ${NAME}
