# Char2Ascii
# Tool to dump ascii values for given strings
# Copyright (C) 2021 - Ethan Marshall

SRC = c2a.c
OBJ = ${SRC:.c=.o}

EXE = ascii
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
