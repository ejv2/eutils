# wsig - wait for a signal
# Copyright (C) 2022 - Ethan Marshall

SRC = wsig.c
OBJ = ${SRC:.c=.o}
EXE = wsig

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LDFLAGS} ${OBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

.PHONY: clean
