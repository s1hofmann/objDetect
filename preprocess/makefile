##################################################
# Author: Simon Hofmann <mail@simon-hofmann.org>
# Date:   2014-08-13
##################################################

NAME 	 = opencv_preprocess
FLAGS    += `pkg-config --libs --cflags opencv`
CFLAGS   += -g -std=c++11
CC 	     = g++

Eval: ./bin/main.o ./bin/preprocess.o ./bin/exception.o ./bin/parser.o
	${CC} ${FLAGS} ${CFLAGS} ./bin/main.o ./bin/preprocess.o ./bin/exception.o ./bin/parser.o -o ${NAME}

bin/main.o: ./src/main.cpp
	${CC} ${CFLAGS} -c ./src/main.cpp -o ./bin/main.o

bin/preprocess.o: ./src/preprocess.cpp
	${CC} ${CFLAGS} -c ./src/preprocess.cpp -o ./bin/preprocess.o

bin/exception.o: ../parser/src/exception.cpp
	${CC} ${CFLAGS} -c ../parser/src/exception.cpp -o ./bin/exception.o

bin/parser.o: ../parser/src/fileParser.cpp
	${CC} ${CFLAGS} -c ../parser/src/fileParser.cpp -o ./bin/parser.o

clean:
	@rm ./bin/*.o
	@rm ${NAME}
