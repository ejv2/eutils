SRC = solve.c parse.c gauss.c
OBJ = ${SRC:.c=.o}
HDR = solve.h

EXE = solve
INS = /usr/local/bin
MLDFLAGS = -lm

${EXE}: ${OBJ}
	${CC} -o ${EXE} ${MLDFLAGS} ${LDFLAGS} ${OBJ}

${OBJ}: ${HDR}

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
