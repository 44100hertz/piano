CC?=g++
RM=rm -f
CFLAGS+=-std=gnu99 -g -Wall
CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS?=$(shell sdl2-config --libs)
LDFLAGS+=-lm -g
VPATH=src/

SRCS=$(wildcard src/*.c)
OBJS=$(subst .c,.o,$(SRCS))

BIN=bin/test

all: test

test: $(OBJS)
	mkdir -p bin/
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend

clean:
	$(RM) *~ .depend
	$(RM) $(OBJS)
	$(RM) $(BIN)

include .depend

.PHONY: all clean depend
