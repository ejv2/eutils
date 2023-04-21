# wrap - wrap lines to a readable length
# Copyright (C) 2023 - Ethan Marshall
SRC = wrap.c
OBJ = ${SRC:.c=.o}

EXE = wrap

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LDFLAGS} ${OBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

.PHONY: clean
