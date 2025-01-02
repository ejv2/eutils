# limexec - limited time execution
# Copyright (C) 2025 - Ethan Marshall

SRC = limexec.c
OBJ = ${SRC:.c=.o}
HDR =

EXE = limexec
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

.PHONY: clean
