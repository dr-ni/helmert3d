CC=gcc
CFLAGS?=-Wall -O2
LDFLAGS?=-lm #-s # - strip for win
AR?=ar
INSTALL?=install
RM?=rm -f # not works for win
RMD?=$(RM) -r # not works for win
PREFIX?=/usr/local
SRCS=src
DOCS=/usr/share/doc/helmert3d
BINS=bin
EXAMPS=examples
MANS=man/man1

all: helmert3d helmparms3d

libsvdm.a: $(SRCS)/svdm.c
	$(CC) $(CFLAGS) -c -o $(SRCS)/svdm.o $^
	$(AR) rcs $@ $(SRCS)/svdm.o

helmert3d: $(SRCS)/helmert3d.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

helmparms3d: $(SRCS)/helmparms3d.c libsvdm.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean: # not works for win
	$(RM) helmparms3d helmert3d libsvdm.a $(SRCS)/svdm.o

install:
	$(INSTALL) -d $(PREFIX)/$(BINS)
	$(INSTALL) -d $(PREFIX)/$(MANS)
	$(INSTALL) -d $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0755 helmparms3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmert3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_src.txt $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_dest.txt $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0644 $(MANS)/helmparms3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmert3d.1 $(PREFIX)/$(MANS)

uninstall:
	$(RM) $(PREFIX)/$(BINS)/helmparms3d
	$(RM) $(PREFIX)/$(BINS)/helmert3d
	$(RMD) $(DOCS)
	$(RM) $(PREFIX)/$(MANS)/helmparms3d.1
	$(RM) $(PREFIX)/$(MANS)/helmert3d.1
