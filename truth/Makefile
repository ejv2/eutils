# truth - evaluate a truth table
# Copyright (C) 2021 - Ethan Marshall

SRC = truth.c parse.c run.c
OBJ = ${SRC:.c=.o}
HDR = truth.h

EXE = truth
INST = /usr/local/bin

# Completely disable (uncompile) logging
# Apparently it could cause a slowdown, so I use
# this for completely fair benchmarking
ifdef NOLOG
	CFLAGS += "-DNOLOG"
endif

${EXE}: ${OBJ}
	${CC} -lm ${LDFLAGS} -o $@ ${OBJ}

${OBJ}: ${HDR}

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o
	rm -f ${EXE}

install: ${EXE}
	mkdir -p ${INST}
	cp -f ${EXE} ${INST}
	chmod 755 ${INST}/${EXE}

uninstall:
	rm -f ${INST}/${EXE}

.PHONY: clean
