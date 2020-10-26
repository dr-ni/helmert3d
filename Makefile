CC=gcc
CFLAGS=-Wall -g
SRCS=src
MANS=man
DEST=/usr/local/bin/
MANDEST=/usr/share/man/man1

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
	cp $(MANS)/helmparms3d.1 $(MANDEST)
	cp $(MANS)/helmert3d.1 $(MANDEST)

uninstall:
	rm -f $(DEST)/helmparms3d
	rm -f $(DEST)/helmert3d
	rm -f $(MANDEST)/helmparms3d.1
	rm -f $(MANDEST)/helmert3d.1
