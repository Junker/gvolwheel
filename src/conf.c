/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * conf.c
 * Copyright (C) Dmitry Kosenkov 2011 <junker@front.ru>
 * 
 * callbacks.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * callbacks.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib/gstdio.h>
#include <glib/gi18n-lib.h>

#include "conf.h"


void load_config ()
{
	gchar *config_filename;
	config_filename = g_build_filename(g_get_home_dir(),
	                                   ".config", 
	                                   PACKAGE_NAME,
	                                   "gvolwheel.conf",
	                                   NULL);
	GKeyFile *config = g_key_file_new();
		
	if (g_key_file_load_from_file(config, config_filename, G_KEY_FILE_NONE, NULL) == FALSE) return;	
	opt_channel = g_key_file_get_integer(config, "Options", "Channel", NULL);
	strcpy(opt_mixer, g_key_file_get_string(config, "Options", "Mixer", NULL));
	opt_incr = g_key_file_get_integer(config, "Options", "Incr", NULL);
	opt_gnome_icons = g_key_file_get_boolean(config, "Options", "Gnome_Icons", NULL);	
	opt_show_tooltip = g_key_file_get_boolean(config, "Options", "Show_Tooltip", NULL);	

	g_key_file_free(config);
	g_free(config_filename);
	
}

void save_config()
{	
	gchar *config_filename = g_build_filename(g_get_home_dir(),
	                                          ".config",
	                                          GETTEXT_PACKAGE,
	                                          "gvolwheel.conf",
	                                          NULL);

	GKeyFile *config = g_key_file_new();
	
	gchar *config_dir  = g_path_get_dirname(config_filename);
	g_mkdir(config_dir, 0755);
	
	g_key_file_set_integer(config, "Options", "Channel", opt_channel);	  
	g_key_file_set_string(config, "Options", "Mixer", opt_mixer);	  
	g_key_file_set_integer(config, "Options", "Incr", opt_incr);	
	g_key_file_set_boolean(config, "Options", "Gnome_Icons", opt_gnome_icons);	
	g_key_file_set_boolean(config, "Options", "Show_Tooltip", opt_show_tooltip);	
	gchar *buffer = g_key_file_to_data(config, NULL, NULL);
	g_file_set_contents(config_filename, buffer, strlen(buffer), NULL);
	
	g_key_file_free(config);
	g_free(buffer);
	g_free(config_filename);
}