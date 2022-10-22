SRC = solve.c parse.c gauss.c
OBJ = ${SRC:.c=.o}
HDR = solve.h
EXE = solve

MLDFLAGS = -lm

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${MLDFLAGS} ${LDFLAGS} ${OBJ}

${OBJ}: ${HDR}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

.PHONY: clean
