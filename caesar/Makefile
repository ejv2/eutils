# caesar - badly encrypt textual data
# Ethan Marshall - 2022

SRC = caesar.c
OBJ = ${SRC:.c=.o}
HDR = 

EXE = caesar
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
