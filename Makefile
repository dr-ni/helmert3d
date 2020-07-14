CC=gcc
CFLAGS=-Wall -g

all: helmert3d helmparms3d

helmert3d: helmert3d.o
	$(CC) $(CFAGS) -o helmert3d helmert3d.o -lm

helmparms3d: helmparms3d.o svdm.o
	$(CC) $(CFAGS) -o helmparms3d helmparms3d.o svdm.o -lm

clean:
	rm -f helmparms3d helmparms3d.o svdm.o helmert3d helmert3d.o

install:
	cp helmparms3d /usr/local/bin/
	cp helmert3d /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/helmparms3d
	rm -f /usr/local/bin/helmert3d
