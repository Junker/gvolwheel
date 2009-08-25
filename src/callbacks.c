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
 
 
#include <linux/soundcard.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <config.h>

#include "callbacks.h"
#include "actions.h"
 
GtkWindow *confwin;
GtkComboBox *confwin_combo_channel;
GtkEntry *confwin_entry_mixer;
GtkSpinButton *confwin_spin_incr; 
GtkCheckButton *confwin_gnome_icons,*confwin_show_tooltip; 
 
extern int mixer_fd, opt_channel,opt_incr;
extern gboolean opt_gnome_icons,opt_show_tooltip;
extern gchar opt_mixer[100];
extern GtkEventBox *tray_box;

StereoVolume vol;

	 
void on_vol_value_change (GtkRange *range,GtkScrollType step,gpointer user_data)
{
	vol_set(gtk_range_get_value (range));
	update_tray_image();
}

void on_mabout_click (GtkMenuItem *menuitem) 
{
	GtkAboutDialog *dlg = GTK_ABOUT_DIALOG(gtk_about_dialog_new ());
	gtk_about_dialog_set_program_name(dlg,"GVolWheel");
	gtk_about_dialog_set_version (dlg,VERSION);
	gtk_about_dialog_set_copyright (dlg,"Copyright (c) Dmitry Kosenkov 2008");
	gtk_about_dialog_set_website (dlg,"http://sourceforge.net/projects/gvolwheel/");
	gtk_about_dialog_set_comments (dlg,	"Volume mixer"); 
	gtk_dialog_run (GTK_DIALOG (dlg));
	gtk_widget_destroy (GTK_WIDGET(dlg));
}

void on_mconfig_click (GtkMenuItem *menuitem) 
{
	confwin = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_window_set_position (confwin,GTK_WIN_POS_CENTER);
	gtk_window_set_destroy_with_parent(confwin,FALSE);
	
	gtk_container_set_border_width(GTK_CONTAINER(confwin),10);
	
	GtkVBox *vbox = GTK_VBOX(gtk_vbox_new (1,5));
	GtkHBox *hbox1 = GTK_HBOX(gtk_hbox_new (0,2));
	
	
	confwin_combo_channel = GTK_COMBO_BOX(gtk_combo_box_new_text ());
	gtk_combo_box_append_text(confwin_combo_channel,"master");  
	gtk_combo_box_append_text(confwin_combo_channel,"pcm");
	gtk_combo_box_set_active (confwin_combo_channel,opt_channel);
	gtk_box_pack_start (GTK_BOX(hbox1),GTK_WIDGET(gtk_label_new ("Default channel")),0,0,5);
	gtk_box_pack_end (GTK_BOX(hbox1),GTK_WIDGET(confwin_combo_channel),0,0,5);
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox1),0,0,0);
 
	GtkHBox *hbox2 = GTK_HBOX(gtk_hbox_new (0,2)); 
	
	confwin_entry_mixer = GTK_ENTRY(gtk_entry_new ());
	gtk_entry_set_text (confwin_entry_mixer,opt_mixer);
	gtk_box_pack_start (GTK_BOX(hbox2),GTK_WIDGET(gtk_label_new("Default mixer")),0,0,5);
	gtk_box_pack_end (GTK_BOX(hbox2),GTK_WIDGET(confwin_entry_mixer),0,0,5);	
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox2),0,0,0);
		
	GtkHBox *hbox3 = GTK_HBOX(gtk_hbox_new (0,2)); 
	confwin_spin_incr = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range (1, 99, 1));
	gtk_spin_button_set_value (confwin_spin_incr,opt_incr);
	gtk_box_pack_start (GTK_BOX(hbox3),GTK_WIDGET(gtk_label_new("Volume incrementation")),0,0,5);
	gtk_box_pack_end (GTK_BOX(hbox3),GTK_WIDGET(confwin_spin_incr),0,0,5);										
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox3),0,0,0);

	GtkHBox *hbox4 = GTK_HBOX(gtk_hbox_new (0,2)); 
	confwin_gnome_icons = GTK_CHECK_BUTTON(gtk_check_button_new_with_label ("Use Gnome Icons"));
	gtk_toggle_button_set_active  (GTK_TOGGLE_BUTTON(confwin_gnome_icons),opt_gnome_icons);
	gtk_box_pack_end (GTK_BOX(hbox4),GTK_WIDGET(confwin_gnome_icons),0,0,5);										
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox4),0,0,0);
	
	GtkHBox *hbox5 = GTK_HBOX(gtk_hbox_new (0,2)); 
	confwin_show_tooltip = GTK_CHECK_BUTTON(gtk_check_button_new_with_label ("Show tooltip"));
	gtk_toggle_button_set_active  (GTK_TOGGLE_BUTTON(confwin_show_tooltip),opt_show_tooltip);
	gtk_box_pack_end (GTK_BOX(hbox5),GTK_WIDGET(confwin_show_tooltip),0,0,5);										
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox5),0,0,0);

	
	GtkHBox *hbox6 = GTK_HBOX(gtk_hbox_new (0,2)); 
	GtkButton *btn_cancel = GTK_BUTTON(gtk_button_new_from_stock ("gtk-cancel"));
	g_signal_connect(G_OBJECT(btn_cancel),"clicked",G_CALLBACK(on_config_window_close),NULL);
	GtkButton *btn_ok = GTK_BUTTON(gtk_button_new_from_stock ("gtk-save"));
	g_signal_connect(G_OBJECT(btn_ok),"clicked",G_CALLBACK(on_config_window_save),NULL);

	gtk_box_pack_start (GTK_BOX(hbox6),GTK_WIDGET(btn_cancel),0,0,5);
	gtk_box_pack_end (GTK_BOX(hbox6),GTK_WIDGET(btn_ok),0,0,5);
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(hbox6),0,0,0);
	
	gtk_container_add(GTK_CONTAINER(confwin),GTK_WIDGET(vbox));
	gtk_widget_show_all (GTK_WIDGET(confwin));
	
}

void on_config_window_close(GtkButton *button,gpointer user_data) 
{
	gtk_widget_destroy(GTK_WIDGET(confwin));
}

void on_config_window_save(GtkButton *button,gpointer user_data) 
{
	opt_channel = gtk_combo_box_get_active(confwin_combo_channel);
	strcpy(opt_mixer,gtk_entry_get_text (confwin_entry_mixer));
	opt_incr = gtk_spin_button_get_value_as_int (confwin_spin_incr);
	opt_gnome_icons = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(confwin_gnome_icons));
	opt_show_tooltip = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(confwin_show_tooltip));
	if (!opt_show_tooltip) 	gtk_widget_set_tooltip_text(GTK_WIDGET(tray_box),"");
	save_config ();
	gtk_widget_destroy(GTK_WIDGET(confwin));
}

void on_tray_icon_scroll (GtkWidget *widget,GdkEventScroll *event,gpointer user_data) 
{
	if (event->direction == GDK_SCROLL_UP) vol_up();
    if (event->direction == GDK_SCROLL_DOWN) vol_down();
	update_tray_image();
}

void on_tray_icon_click(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
	if (event->button == 3) tray_icon_menu_show();
}

void on_tray_icon_press(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
	if (event->button == 1 && event->type == GDK_BUTTON_PRESS) vol_window_show();
	if (event->button == 1 && event->type == GDK_2BUTTON_PRESS) launch_mixer();
}

