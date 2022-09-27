# endian - print machine endianness
# Ethan Marshall - 2022

SRC = endian.c
OBJ = ${SRC:.c=.o}

EXE = endian
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

.PHONY: clean install uninstall
