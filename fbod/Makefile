# fbox - draw labelled free body force diagrams
# Copyright (C) 2022 - Ethan Marshall
SRC = fbod.c
OBJ = ${SRC:.c=.o}

EXE = fbod
INS = /usr/local/bin

LIBS = -lm

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${LIBS} ${LDFLAGS} ${OBJ}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${EXE} *.o

install: ${EXE}
	mkdir -p ${INS}
	cp -f ${EXE} ${INS}
	chmod 755 ${INS}/${EXE}

uninstall:
	rm -f ${INS}/${EXE}

.PHONY: clean install uninstall
