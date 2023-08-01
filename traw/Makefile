SRC = traw.c
OBJ = ${SRC:.c=.o}
EXE = traw

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LDFLAGS} ${OBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

.PHONY: clean
