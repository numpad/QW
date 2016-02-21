CC=gcc
CSTD=c99
CLIB=-lSDL2 -lSDL2_image -lSDL2_gfx -lm

input=qtest
output=${input}

all:
	${CC} -std=${CSTD} ${input}.c -o${output} ${CLIB}

