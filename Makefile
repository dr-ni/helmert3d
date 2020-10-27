CC=gcc
CFLAGS=-Wall -g
SRCS=src
PREFIX=/usr/local
BINS=bin
MANS=man/man1
INSTALL=install
RM=rm -f

all: helmert3d helmparms3d

helmert3d: $(SRCS)/helmert3d.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

helmparms3d: $(SRCS)/helmparms3d.c $(SRCS)/svdm.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f helmparms3d helmert3d

install:
	$(INSTALL) -d $(PREFIX)/$(BINS)
	$(INSTALL) -d $(PREFIX)/$(MANS)
	$(INSTALL) -m 0755 helmparms3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmert3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0644 $(MANS)/helmparms3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmert3d.1 $(PREFIX)/$(MANS)

uninstall:
	$(RM) $(PREFIX)/$(BINS)/helmparms3d
	$(RM) $(PREFIX)/$(BINS)/helmert3d
	$(RM) $(PREFIX)/$(MANS)/helmparms3d.1
	$(RM) $(PREFIX)/$(MANS)/helmert3d.1
