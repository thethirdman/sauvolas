LFLAGS=`pkg-config --libs opencv` `pkg-config --libs tbb`
CFLAGS=-O3 -std=c++11
CC=g++


BINS=opti para slow

all: sauvola.o ${BINS}

sauvola.o: src/sauvola.cc
	${CC} ${CFLAGS} ${LFLAGS} -c $< -o $@

opti:
	${CC} ${CFLAGS} ${LFLAGS} $(addprefix demo/, $@.cc)  sauvola.o -o $@
para:
	${CC} ${CFLAGS} ${LFLAGS} $(addprefix demo/, $@.cc)  sauvola.o -o $@
slow:
	${CC} ${CFLAGS} ${LFLAGS} $(addprefix demo/, $@.cc)  sauvola.o -o $@

clean:
	rm -f sauvola.o opti para slow
