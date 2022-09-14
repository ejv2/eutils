# bexp - binary explorer
# Ethan Marshall - 2022

SRC = bexp.c
OBJ = ${SRC:.c=.o}

EXE = bexp
INST = /usr/local/bin

${EXE}: ${OBJ}
	${CC} ${LDFLAGS} -o ${EXE} ${OBJ}

.c.o:
	${CC} ${CFLAGS} -c  $<

clean:
	rm -f *.o ${EXE}

install: ${EXE}
	mkdir -p ${INST}
	cp -f ${EXE} ${INST}
	chmod 755 ${INST}/${EXE}

uninstall:
	rm -f ${INST}/${EXE}

.PHONY = clean install uninstall
