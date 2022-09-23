# rads - convert to and from radians and degrees
# Ethan Marshall - 2022

RSRC = rads.c
ROBJ = ${RSRC:.c=.o}
DSRC = degs.c
DOBJ = ${DSRC:.c=.o}

REXE = rads
DEXE = degs
INST = /usr/local/bin

all: ${REXE} ${DEXE}

${REXE}: ${ROBJ}
	${CC} ${LDFLAGS} -o $@ ${ROBJ}

${DEXE}: ${DOBJ}
	${CC} ${LDFLAGS} -o $@ ${DOBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm *.o
	rm ${REXE} ${DEXE}

install: ${DEXE} ${REXE}
	mkdir -p ${INST}
	cp -f ${DEXE} ${REXE} ${INST}
	chmod 755 ${INST}/${EXE}

uninstall:
	rm -f ${INST}/${EXE}

.PHONY = clean
