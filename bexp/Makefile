# bexp - binary explorer
# Ethan Marshall - 2022

SRC = bexp.c
SRC2 = bconv.c
OBJ = ${SRC:.c=.o}
OBJ2 = ${SRC2:.c=.o}

EXE = bexp
EXE2 = bconv
INST = /usr/local/bin

all: ${EXE} ${EXE2}

${EXE}: ${OBJ}
	${CC} ${LDFLAGS} -o ${EXE} ${OBJ}

${EXE2}: ${OBJ2}
	${CC} ${LDFLAGS} -o ${EXE2} ${OBJ2}

.c.o:
	${CC} ${CFLAGS} -c  $<

clean:
	rm -f *.o ${EXE} ${EXE2}

install: ${EXE}
	mkdir -p ${INST}
	cp -f ${EXE} ${INST}
	chmod 755 ${INST}/${EXE}

uninstall:
	rm -f ${INST}/${EXE}

.PHONY = clean install uninstall
