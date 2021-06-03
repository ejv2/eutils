# Memory destroyer
# A program that allocates way too much memory
# Copyright (C) 2021 - Ethan Marshall

SRC = main.c
OBJ = ${SRC:.c=.o}

EXE = mdest
INS = /usr/local/bin

${EXE}: ${OBJ}
	${CC} ${LDFLAGS} -o ${EXE} ${OBJ}

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm *.o
	rm ${EXE}

install: ${EXE}
	mkdir -p ${INS}
	cp -f ${EXE} ${INS}
	chmod 755 ${INS}/${EXE}

uninstall:
	rm -f ${INS}/${EXE}

.PHONY = clean install uninstall
