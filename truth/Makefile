# truth - evaluate a truth table
# Copyright (C) 2021 - Ethan Marshall

SRC = truth.c parse.c
OBJ = ${SRC:.c=.o}
HDR = 

EXE = truth
INST = /usr/local/bin

${EXE}: ${OBJ}
	${CC} ${LDFLAGS} -o $@ ${OBJ}

${OBJ}: ${HDR}

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm *.o
	rm ${EXE}

install: ${EXE}
	mkdir -p ${INST}
	cp -f ${EXE} ${INST}
	chmod 755 ${INST}/${EXE}

uninstall:
	rm -f ${INST}/${EXE}

.PHONY = clean
