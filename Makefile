CC=gcc
CFLAGS=-Wall -g

all: helmert3d helmparms3d

helmert3d: helmert3d.c
	$(CC) $(CFAGS) -o helmert3d helmert3d.c -lm

helmparms3d: helmparms3d.c svdm.c
	$(CC) $(CFAGS) -o helmparms3d helmparms3d.c svdm.c -lm

clean:
	rm -f helmparms3d helmert3d

install:
	cp helmparms3d /usr/local/bin/
	cp helmert3d /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/helmparms3d
	rm -f /usr/local/bin/helmert3d
