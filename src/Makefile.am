## Process this file with automake to produce Makefile.in

## Created by Anjuta

gladedir = $(datadir)/gvolwheel/glade
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GTK_CFLAGS) \
        ${ALSA_CFLAGS}

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gvolwheel

if BACKEND_ALSA
BACKEND = alsa.c alsa.h
else
BACKEND = oss.c oss.h
endif

gvolwheel_SOURCES = \
	main.c \
	actions.c \
	actions.h \
	callbacks.c \
	callbacks.h \
	conf.c \
	conf.h \
	volume.c \
	volume.h \
	configwindow.c \
	configwindow.h \
	trayicon.c \
	trayicon.h \
        $(BACKEND)

gvolwheel_LDFLAGS = \
	-Wl,--export-dynamic

gvolwheel_LDADD = $(GTK_LIBS) ${ALSA_LIBS}

EXTRA_DIST = $(glade_DATA)
