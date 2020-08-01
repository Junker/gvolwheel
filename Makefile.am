## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

gvolwheeldocdir = ${prefix}/doc/gvolwheel
gvolwheeldoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(gvolwheeldoc_DATA) \
	$(intltool_DATA)\
	$(pixmaps_DATA)\
	$(intltool_DATA)

pixmapsdir = ${prefix}/share/pixmaps/gvolwheel
pixmaps_DATA = \
	pixmaps/audio-volume-high.png\
	pixmaps/audio-volume-low.png \
	pixmaps/audio-volume-medium.png \
	pixmaps/audio-volume-muted.png

intltooldir = /dev/null
# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done
