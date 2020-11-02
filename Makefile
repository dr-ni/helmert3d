CC=gcc
CFLAGS=-Wall -O2
SRCS=src
PREFIX=/usr/local
DATAP=share/helmparms3d
BINS=bin
EXAMPS=examples
MANS=man/man1
INSTALL=install
RM=rm -f
RMD=rm -rf
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
	$(INSTALL) -d $(PREFIX)/$(DATAP)
	$(INSTALL) -m 0755 helmparms3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmert3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_src.txt $(PREFIX)/$(DATAP)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_dest.txt $(PREFIX)/$(DATAP)
	$(INSTALL) -m 0644 $(MANS)/helmparms3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmert3d.1 $(PREFIX)/$(MANS)

uninstall:
	$(RM) $(PREFIX)/$(BINS)/helmparms3d
	$(RM) $(PREFIX)/$(BINS)/helmert3d
	$(RMD) $(PREFIX)/$(DATAP)
	$(RM) $(PREFIX)/$(MANS)/helmparms3d.1
	$(RM) $(PREFIX)/$(MANS)/helmert3d.1
