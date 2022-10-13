SRC = solve.c parse.c
OBJ = ${SRC:.c=.o}
HDR = solve.h
EXE = solve

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LDFLAGS} ${OBJ}

${OBJ}: ${HDR}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

.PHONY: clean
