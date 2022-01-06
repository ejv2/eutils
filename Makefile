# EUT - Ethan's UTils
# A small collection of (semi-)useful utilities and programs

DIRS = memdest/ rand/ wordlen/ char2ascii/ tally/ truth/

export EXES
export SRC
include $(addsuffix /Sources.inc, ${DIRS})

export CFLAGS ?= -Wall -Wpedantic
export LDFLAGS ?= -lm

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

all: ${EXES}

${EXES}: ${SRC}
	${MAKE} -C $(dir $@)

clean: ${DIRS}
	-rm $(addsuffix *.o,${DIRS})
	rm ${EXES}

.PHONY = all clean
