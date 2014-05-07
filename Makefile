# Makefile for Intel .hex file parser
#

example: example.c ihex.c ihex.h
	${CC} ${CFLAGS} $^ -o $@

.PHONY: test clean

test: example example.hex
	cat example.hex | ./example

clean:
	rm -f example

