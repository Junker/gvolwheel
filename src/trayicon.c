/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * trayicon.c
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

#include "trayicon.h"
#include "conf.h"
#include "actions.h"
#include "volume.h"

const char *tray_image_stocks[] = {
	 "audio-volume-muted",
	 "audio-volume-low",
	 "audio-volume-medium",
	 "audio-volume-high"
 	 };


GtkStatusIcon *create_tray_icon()
{
	tray_icon = gtk_status_icon_new();

	if (opt_gnome_icons==TRUE)
		gtk_status_icon_set_from_icon_name(tray_icon, tray_image_stocks[1]);
	else load_pixbufs(), gtk_status_icon_set_from_pixbuf(tray_icon, tray_pixbufs[2]);

	update_tray_image();

	g_signal_connect(G_OBJECT(tray_icon), "button-release-event", G_CALLBACK(on_tray_icon_click), NULL);
	g_signal_connect(G_OBJECT(tray_icon), "button-press-event", G_CALLBACK(on_tray_icon_press), NULL);
	g_signal_connect(G_OBJECT(tray_icon), "scroll-event", G_CALLBACK(on_tray_icon_scroll), NULL);
//?	g_signal_connect(G_OBJECT(tray_icon), "destroy", G_CALLBACK(on_tray_icon_destroyed), NULL);

	gtk_status_icon_set_visible(tray_icon, TRUE);


	return tray_icon;
}


void load_pixbufs()
{
	GtkImage *tmp_image;
	gchar *tmp;
	int i;

	gchar *pixmap_path = g_build_filename(PACKAGE_DATA_DIR, "pixmaps", PACKAGE_NAME,NULL);

	for (i = 0; i < 4; i++)
	{
		tmp = g_build_filename(g_get_home_dir(),
		                       ".config",
		                       PACKAGE_NAME,
		                       "pixmaps",
		                       tray_image_stocks[i],
		                       NULL
		                      );
		gchar *image_filename_local = g_strconcat(tmp, ".png", NULL);
		g_free(tmp);

		tmp = g_build_filename(pixmap_path,
		                       tray_image_stocks[i],
		                       NULL);
		gchar *image_filename = g_strconcat(tmp, ".png", NULL);
		g_free(tmp);

		if (g_file_test(image_filename_local, G_FILE_TEST_EXISTS))
			tmp_image = GTK_IMAGE(gtk_image_new_from_file(image_filename_local));
		else
			tmp_image = GTK_IMAGE(gtk_image_new_from_file(image_filename));


		tray_pixbufs[i] = gtk_image_get_pixbuf (tmp_image);

		g_free(image_filename_local);
		g_free(image_filename);
	}
	g_free(pixmap_path);

}

gboolean tray_icon_recreate(gpointer data)
{
	tray_icon = create_tray_icon();
	return FALSE;
}

void on_tray_icon_destroyed(GtkWidget *widget, void *data)
{
	g_object_unref(G_OBJECT(tray_icon));
	tray_icon = NULL;

	g_idle_add(tray_icon_recreate, NULL);
}

void on_tray_icon_scroll (GtkWidget *widget, GdkEventScroll *event, gpointer user_data)
{
	if (event->direction == GDK_SCROLL_UP) vol_up();
    if (event->direction == GDK_SCROLL_DOWN) vol_down();
	update_tray_image();
}

void on_tray_icon_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 3) tray_icon_menu_show();
	if (event->button == 2)
	{
		vol_mute();
		update_tray_image();
	}
}

void on_tray_icon_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 1 && event->type == GDK_BUTTON_PRESS) vol_window_show();
	if (event->button == 1 && event->type == GDK_2BUTTON_PRESS) launch_mixer();
}

void update_tray_image()
{
	gint vol = vol_get();
	gchar volchar[5];
	GdkPixbuf *tmp_pixbuf;

	if (!opt_gnome_icons)
	{
		if (gtk_status_icon_get_storage_type(tray_icon) == GTK_IMAGE_PIXBUF) tmp_pixbuf = gtk_status_icon_get_pixbuf(tray_icon);
		if (vol>0  && vol<=33  && tmp_pixbuf!=tray_pixbufs[1]) gtk_status_icon_set_from_pixbuf(tray_icon, tray_pixbufs[1]);
    	if (vol>33 && vol<=66  && tmp_pixbuf!=tray_pixbufs[2]) gtk_status_icon_set_from_pixbuf(tray_icon, tray_pixbufs[2]);
    	if (vol>66 && vol<=100 && tmp_pixbuf!=tray_pixbufs[3]) gtk_status_icon_set_from_pixbuf(tray_icon, tray_pixbufs[3]);
    	if (vol==0 &&             tmp_pixbuf!=tray_pixbufs[0]) gtk_status_icon_set_from_pixbuf(tray_icon, tray_pixbufs[0]);
	}
	else {
		const gchar *icon_name = "";
		if (gtk_status_icon_get_storage_type(tray_icon) == GTK_IMAGE_STOCK) icon_name = gtk_status_icon_get_icon_name(tray_icon);
		if (vol>0  && vol<=33  && strcmp(icon_name, tray_image_stocks[1]) != 0) gtk_status_icon_set_from_icon_name(tray_icon, tray_image_stocks[1]);
    	if (vol>33 && vol<=66  && strcmp(icon_name, tray_image_stocks[2]) != 0) gtk_status_icon_set_from_icon_name(tray_icon, tray_image_stocks[2]);
   		if (vol>66 && vol<=100 && strcmp(icon_name, tray_image_stocks[3]) != 0) gtk_status_icon_set_from_icon_name(tray_icon, tray_image_stocks[3]);
    	if (vol==0 &&             strcmp(icon_name, tray_image_stocks[0]) != 0) gtk_status_icon_set_from_icon_name(tray_icon, tray_image_stocks[0]);
	}

	g_sprintf(volchar, "%i\%%", vol);
	if (opt_show_tooltip) gtk_status_icon_set_tooltip_text(tray_icon, volchar);
}
