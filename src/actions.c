/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * actions.c
 * Copyright (C) Dmitry Kosenkov 2008 <junker@front.ru>
 * 
 * actions.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * actions.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>



#include "actions.h"
#include "conf.h"
#include "callbacks.h"


GtkWindow *vwin;
 
 
void tray_icon_menu_show()
{
    GtkMenu *menu = GTK_MENU(gtk_menu_new());
	GtkWidget *mprefs = gtk_image_menu_item_new_from_stock("gtk-preferences",NULL);
	GtkWidget *mabout = gtk_image_menu_item_new_from_stock("gtk-about",NULL);
	GtkWidget *mquit  = gtk_image_menu_item_new_from_stock("gtk-quit",NULL);
	

	g_signal_connect(G_OBJECT(mprefs), "activate", G_CALLBACK(on_mconfig_click),NULL);
	g_signal_connect(G_OBJECT(mabout), "activate", G_CALLBACK(on_mabout_click),NULL);
	g_signal_connect(G_OBJECT(mquit), "activate", G_CALLBACK(gtk_main_quit),NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mprefs);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mabout);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new ());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mquit);
	
	gtk_widget_show_all (GTK_WIDGET(menu));
	gtk_menu_popup (menu, NULL, NULL, NULL, NULL, 0,																0);
}

void launch_mixer () 
{
	char sys[100];
	sprintf(sys,"%s %c", opt_mixer, '&');
	system(sys);
}



void vol_window_show ()
{
	if (!GTK_IS_WINDOW(vwin)) vwin = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	else {
		vol_window_close();	
		return;
	}
	
	gtk_container_set_border_width(GTK_CONTAINER(vwin), 5);
	gtk_window_set_decorated (vwin, FALSE);
	gtk_window_set_position (vwin, GTK_WIN_POS_MOUSE);
	gtk_window_set_skip_taskbar_hint(vwin, TRUE);
	gtk_window_set_skip_pager_hint (vwin, TRUE);
	                                
		
	g_signal_connect(G_OBJECT(vwin), "focus-out-event", G_CALLBACK(vol_window_close), NULL);
	
	GtkVBox *vbox = GTK_VBOX(gtk_vbox_new (0, 4));
	
	GtkButton *btn = GTK_BUTTON(gtk_button_new_with_label (_("Mixer")));
	g_signal_connect(G_OBJECT(btn),"clicked",G_CALLBACK(launch_mixer), NULL);
	
	gtk_box_pack_start (GTK_BOX(vbox), GTK_WIDGET(btn), 0, 0, 0);
	
	gtk_box_pack_start (GTK_BOX(vbox), gtk_hseparator_new(), 0, 0, 0);
	
	GtkVScale *vscale = GTK_VSCALE(gtk_vscale_new_with_range (0, 100, 1));
	gtk_scale_set_digits (GTK_SCALE(vscale), 0);
	gtk_scale_set_value_pos (GTK_SCALE(vscale), GTK_POS_BOTTOM);
	gtk_range_set_inverted (GTK_RANGE(vscale), TRUE);
	gtk_widget_set_size_request(GTK_WIDGET(vscale), 0, 120);

	gtk_range_set_value (GTK_RANGE(vscale), vol_get());
	g_signal_connect(vscale, "value-changed", G_CALLBACK(on_vol_value_change), NULL);
	
	gtk_box_pack_start (GTK_BOX(vbox), GTK_WIDGET(vscale),0,0,0);
	
	GtkLabel *label = GTK_LABEL(gtk_label_new("Master"));
	if (opt_channel == OPT_CHANNEL_MASTER) gtk_label_set_label (label, "Master");
	if (opt_channel == OPT_CHANNEL_PCM) gtk_label_set_label (label, "PCM");
	gtk_box_pack_start (GTK_BOX(vbox), GTK_WIDGET(label), 0, 0, 0);
	
	gtk_container_add(GTK_CONTAINER(vwin), GTK_WIDGET(vbox));
	gtk_widget_show_all (GTK_WIDGET(vwin));
	gtk_window_set_focus (vwin, NULL);
}

void vol_window_close()
{
	gtk_widget_destroy(GTK_WIDGET(vwin));
}


