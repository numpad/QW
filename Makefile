CC=gcc
CSTD=c99
CLIB=-lSDL2 -lm

input=qtest
output=${input}

all:
	${CC} -std=${CSTD} ${input}.c -o${output} ${CLIB}

