CC=gcc
CFLAGS=-Wall -g
SRCS=src
DEST=/usr/local/bin/

all: helmert3d helmparms3d

helmert3d: $(SRCS)/helmert3d.c
	$(CC) $(CFAGS) -o $@ $^ -lm

helmparms3d: $(SRCS)/helmparms3d.c $(SRCS)/svdm.c
	$(CC) $(CFAGS) -o $@ $^ -lm

clean:
	rm -f helmparms3d helmert3d

install:
	cp helmparms3d $(DEST)
	cp helmert3d $(DEST)

uninstall:
	rm -f $(DEST)/helmparms3d
	rm -f $(DEST)/helmert3d
