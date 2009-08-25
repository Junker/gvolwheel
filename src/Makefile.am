## Process this file with automake to produce Makefile.in

## Created by Anjuta

gladedir = $(datadir)/gvolwheel/glade
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GVOLWHEEL_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gvolwheel

gvolwheel_SOURCES = \
	main.c \
	eggtrayicon.c \
	eggtrayicon.h \
	actions.c \
	actions.h \
	callbacks.c \
	callbacks.h

gvolwheel_LDFLAGS = \
	-Wl,--export-dynamic

gvolwheel_LDADD = $(GVOLWHEEL_LIBS)

EXTRA_DIST = $(glade_DATA)
