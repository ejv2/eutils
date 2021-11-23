# EUT - Ethan's UTils
# A small collection of (semi-)useful utilities and programs

DIRS = memdest/ rand/ wordlen/ char2ascii/ tally/ truth/

export EXES
include $(addsuffix /Sources.inc, ${DIRS})

export CFLAGS ?= -std=c99 -Wall -Wpedantic
export LDFLAGS ?= -lm

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -O2
endif

all: ${EXES} FORCE

${EXES}:
	${MAKE} -C $(dir $@)

clean: ${DIRS}
	-rm $(addsuffix *.o,${DIRS})
	rm ${EXES}

FORCE:

.PHONY = all clean FORCE
