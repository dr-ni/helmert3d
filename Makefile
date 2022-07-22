CC=gcc
CFLAGS?=-Wall -O2
LDFLAGS?=-lm #-s # - strip for win
AR?=ar
INSTALL?=install
RM?=rm -f # not working for win
RMD?=$(RM) -r # not working for win
PREFIX?=/usr/local
SRCS=src
DOCS=/usr/share/doc/helmert3d
BINS=bin
EXAMPS=examples
ELLIPS=ellipsoids
ZONES=zones
MANS=man/man1

all: helmert3d helmparms3d helmdiff3d helmeltrans helmtmerc

libsvdm.a: $(SRCS)/svdm.c
	$(CC) $(CFLAGS) -c -o $(SRCS)/svdm.o $^
	$(AR) rcs $@ $(SRCS)/svdm.o

helmert3d: $(SRCS)/helmert3d.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

helmparms3d: $(SRCS)/helmparms3d.c libsvdm.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

helmdiff3d: $(SRCS)/helmdiff3d.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

helmeltrans: $(SRCS)/helmeltrans.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

helmtmerc: $(SRCS)/helmtmerc.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

pdfman: # not working for win
	groff -m man -T ps $(MANS)/helmert3d.1 > man_helmert3d.ps
	ps2pdf man_helmert3d.ps man_helmert3d.pdf
	groff -m man -T ps $(MANS)/helmparms3d.1 > man_helmparms3d.ps
	ps2pdf man_helmparms3d.ps man_helmparms3d.pdf
	groff -m man -T ps $(MANS)/helmdiff3d.1 > man_helmdiff3d.ps
	ps2pdf man_helmdiff3d.ps man_helmdiff3d.pdf
	groff -m man -T ps $(MANS)/helmeltrans.1 > man_helmeltrans.ps
	ps2pdf man_helmeltrans.ps man_helmeltrans.pdf
	groff -m man -T ps $(MANS)/helmtmerc.1 > man_helmtmerc.ps
	ps2pdf man_helmtmerc.ps man_helmtmerc.pdf
	$(RM) *.ps

clean: # not working for win
	$(RM) helmparms3d helmert3d helmdiff3d helmeltrans helmtmerc libsvdm.a $(SRCS)/svdm.o *.pdf

install:
	$(INSTALL) -d $(PREFIX)/$(BINS)
	$(INSTALL) -d $(PREFIX)/$(MANS)
	$(INSTALL) -d $(DOCS)/$(EXAMPS)
	$(INSTALL) -d $(DOCS)/$(ELLIPS)
	$(INSTALL) -d $(DOCS)/$(ZONES)
	$(INSTALL) -m 0755 helmparms3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmert3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmdiff3d $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmeltrans $(PREFIX)/$(BINS)
	$(INSTALL) -m 0755 helmtmerc $(PREFIX)/$(BINS)
	$(INSTALL) -m 0644 README.md $(DOCS)
	$(INSTALL) -m 0644 LICENSE $(DOCS)
	$(INSTALL) -m 0644 common_ellipsoids.txt $(DOCS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_src.txt $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_dest.txt $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0644 $(EXAMPS)/testpoints_blh.txt $(DOCS)/$(EXAMPS)
	$(INSTALL) -m 0644 $(ELLIPS)/BESSEL.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ELLIPS)/GRS80.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ELLIPS)/GSK2011.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ELLIPS)/KRASOVSKIY.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ELLIPS)/PZ90.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ELLIPS)/WGS84.txt $(DOCS)/$(ELLIPS)
	$(INSTALL) -m 0644 $(ZONES)/6d4zone.txt $(DOCS)/$(ZONES)
	$(INSTALL) -m 0644 $(ZONES)/6d4zonespec.txt $(DOCS)/$(ZONES)
	$(INSTALL) -m 0644 $(MANS)/helmparms3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmert3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmdiff3d.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmeltrans.1 $(PREFIX)/$(MANS)
	$(INSTALL) -m 0644 $(MANS)/helmtmerc.1 $(PREFIX)/$(MANS)

uninstall:
	$(RM) $(PREFIX)/$(BINS)/helmparms3d
	$(RM) $(PREFIX)/$(BINS)/helmert3d
	$(RM) $(PREFIX)/$(BINS)/helmdiff3d
	$(RM) $(PREFIX)/$(BINS)/helmeltrans
	$(RM) $(PREFIX)/$(BINS)/helmtmerc
	$(RMD) $(DOCS)
	$(RM) $(PREFIX)/$(MANS)/helmparms3d.1
	$(RM) $(PREFIX)/$(MANS)/helmert3d.1
	$(RM) $(PREFIX)/$(MANS)/helmdiff3d.1
	$(RM) $(PREFIX)/$(MANS)/helmeltrans.1
	$(RM) $(PREFIX)/$(MANS)/helmtmerc.1
