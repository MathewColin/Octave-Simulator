# Copyright 2020 Darius Neatu <neatudarius@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=my_octave

build: $(TARGETS)

my_octave: my_octave.c
	$(CC) $(CFLAGS) my_octave.c -o my_octave
	
clean:
	rm -f $(TARGETS)

pack:
	zip -FSr 311CA_CeausuMateiCalin_Tema2.zip README Makefile *.c *.h

.PHONY: pack clean
