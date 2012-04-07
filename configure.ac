dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(gvolwheel, 1.0, http://gvolwheel.sourceforge.net/, , http://gvolwheel.sourceforge.net/)

AC_SUBST(PACKAGE_DATA_DIR)


AC_SUBST(PACKAGE_PIXMAPS_DIR)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC




dnl ***************************************************************************
dnl Internatinalization
dnl ***************************************************************************
GETTEXT_PACKAGE=gvolwheel
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])



AM_PROG_LIBTOOL

AC_ARG_ENABLE([oss],
  [  --disable-oss    disable OSS],
  [oss=${enableval}],
  [oss=no])
  
AS_IF([test "x$oss" = "xno"], 
[
  PKG_CHECK_MODULES(ALSA,alsa)
  AC_SUBST(ALSA_CFLAGS)
  AC_SUBST(ALSA_LIBS)  
  AC_DEFINE([BACKEND_ALSA], 1, [Alsa backend])
]
)

AM_CONDITIONAL(BACKEND_ALSA, test "$oss" = "no")

PKG_CHECK_MODULES(GTK,gtk+-3.0)

AC_SUBST(GVOLWHEEL_CFLAGS)
AC_SUBST(GVOLWHEEL_LIBS)



AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])
