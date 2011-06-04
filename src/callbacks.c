/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.c
 * Copyright (C) Dmitry Kosenkov 2008 <junker@front.ru>
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

#include "callbacks.h"
#include "conf.h"
#include "trayicon.h"
#include "volume.h"
#include "configwindow.h"

 


	 
void on_vol_value_change (GtkRange *range, GtkScrollType step, gpointer user_data)
{
	vol_set(gtk_range_get_value (range));
	update_tray_image();
}

void on_mconfig_click (GtkMenuItem *menuitem) 
{
	config_window_show();
	
}

void on_mabout_click (GtkMenuItem *menuitem) 
{
	GtkAboutDialog *dlg = GTK_ABOUT_DIALOG(gtk_about_dialog_new ());
	gtk_about_dialog_set_program_name(dlg, "GVolWheel");
	gtk_about_dialog_set_version (dlg,VERSION);
	gtk_about_dialog_set_copyright (dlg, "Copyright (c) Dmitry Kosenkov 2010");
	gtk_about_dialog_set_website (dlg, PACKAGE_URL);
	gtk_about_dialog_set_comments (dlg,	"Volume mixer"); 
	gtk_dialog_run (GTK_DIALOG (dlg));
	gtk_widget_destroy (GTK_WIDGET(dlg));
}


void on_timer(gpointer data)
{
	update_tray_image();

	
}