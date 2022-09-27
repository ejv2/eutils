# fbox - draw labelled free body force diagrams
# Copyright (C) 2022 - Ethan Marshall
SRC = fbod.c
OBJ = ${SRC:.c=.o}

EXE = fbod

LIBS = -lm

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LIBS} ${LDFLAGS} ${OBJ}

.c.o:
	${CC} -c ${CFLAGS} $<
