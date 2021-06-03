# wordlen - show processor word size
# Ethan Marshall - 2021

SRC = main.c
OBJ = ${SRC:.c=.o}

EXE = word
INST = /usr/local/bin

${EXE}: ${OBJ}
	${CC} ${LDFLAGS} -o ${EXE} ${OBJ}

%.o: %.c
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
