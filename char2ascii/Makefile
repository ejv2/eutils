# Char2Ascii
# Tool to dump ascii values for given strings
# Copyright (C) 2021 - Ethan Marshall

SRC = c2a.c
OBJ = ${SRC:.c=.o}

SRCU = unicode.c
OBJU = ${SRCU:.c=.o}

ASCII   = ascii
UNICODE = unicode
INS = /usr/local/bin

all: ${ASCII} ${UNICODE}

${ASCII}: ${OBJ}
	${CC} ${LDFLAGS} -o ${ASCII} ${OBJ}

${UNICODE}: ${OBJU}
	${CC} ${LDFLAGS} -o ${UNICODE} ${OBJU}

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
	rm -f ${ASCII} ${UNICODE}

install: ${EXE}
	mkdir -p ${INS}
	cp -f ${EXE} ${INS}
	chmod 755 ${INS}/${EXE}

uninstall:
	rm -f ${INS}/${EXE}

.PHONY = clean install uninstall
