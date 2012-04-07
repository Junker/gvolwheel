/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Dmitry Kosenkov 2011 <junker@front.ru>
 * 
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
 
#include <config.h>

#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <glib/gi18n-lib.h>


/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include "trayicon.h"
#include "conf.h"
#include "actions.h"
#include "callbacks.h"
#include "volume.h"

static gchar *device = "/dev/mixer"; 
static GOptionEntry entries[] =
{
	{ "device", 'd', 0, G_OPTION_ARG_STRING, &device, "Mixer device (default: /dev/mixer)", "N" },
	{NULL}
};



int main (int argc, char *argv[])
{

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

//gtk3	gtk_set_locale ();
	gtk_init (&argc, &argv);
	
	GError *error = NULL;
	GOptionContext *context;

	context = g_option_context_new ("- tray icon audio volume mixer");
	g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));

	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_print (_("option parsing failed: %s\n"), error->message);
		exit (1);
	}
	g_option_context_free(context);

	

	if (!vol_init()) g_printf (_("Error opening mixer device %s\n"), device), exit (1);
	
	strcpy(opt_mixer, "gnome-alsamixer");
	opt_channel = OPT_CHANNEL_MASTER;
	opt_incr = 3;
	opt_gnome_icons = FALSE;
	opt_show_tooltip = FALSE; 
	
	load_config ();
	tray_icon = create_tray_icon();

	g_timeout_add (1000, (GSourceFunc) on_timer, NULL); //For update icon, if volume changed from other app
	
	gtk_main ();

//	close(mixer_fd);
	return 0;
}


