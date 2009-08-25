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
 
#include <linux/soundcard.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <config.h>

#include "actions.h"
#include "callbacks.h"

#define OPT_CHANNEL_MASTER		0
#define OPT_CHANNEL_PCM		1 
 
extern int mixer_fd;
extern GdkPixbuf *tray_pixbufs[];
extern GtkImage *tray_image; 
extern GtkEventBox *tray_box;
extern char *tray_image_stocks[];
extern StereoVolume vol;
 
GtkWindow *vwin;
gchar opt_mixer[100] ="gnome-alsamixer";
guint opt_channel = OPT_CHANNEL_MASTER;
guint opt_incr = 3;
gboolean opt_gnome_icons = FALSE;
gboolean opt_show_tooltip = FALSE; 
	 

 
 
void vol_up () 
{
	vol_set(vol_get()+opt_incr);
}

void vol_down () 
{
	vol_set(vol_get()-opt_incr);
}

void vol_mute () 
{
	vol_set(0);
}

void vol_set (int value)
{
	if (value<0) value=0;
	vol.left=vol.right = value;
	if (opt_channel == 0)	
		ioctl(mixer_fd,MIXER_WRITE(SOUND_MIXER_VOLUME), &vol);
	else if (opt_channel == 1)	
		ioctl(mixer_fd,MIXER_WRITE(SOUND_MIXER_PCM), &vol);
}

int vol_get ()
{
	if (opt_channel == 0)
		ioctl(mixer_fd,MIXER_READ(SOUND_MIXER_VOLUME), &vol);
	else if (opt_channel == 1)	
		ioctl(mixer_fd,MIXER_READ(SOUND_MIXER_PCM), &vol);
	return(vol.left);
}

void tray_icon_menu_show()
{
    GtkMenu *menu = GTK_MENU(gtk_menu_new());
	GtkWidget *mprefs =  gtk_image_menu_item_new_from_stock("gtk-preferences",NULL);
	GtkWidget *mabout =  gtk_image_menu_item_new_from_stock("gtk-about",NULL);
	GtkWidget *mquit =  gtk_image_menu_item_new_from_stock("gtk-quit",NULL);
	

	g_signal_connect(G_OBJECT(mprefs),"activate",G_CALLBACK(on_mconfig_click),NULL);
	g_signal_connect(G_OBJECT(mabout),"activate",G_CALLBACK(on_mabout_click),NULL);
	g_signal_connect(G_OBJECT(mquit),"activate",G_CALLBACK(gtk_main_quit),NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu),mprefs);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),mabout);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),gtk_separator_menu_item_new ());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),mquit);
	
	gtk_widget_show_all (GTK_WIDGET(menu));
	gtk_menu_popup (menu,NULL,NULL,NULL,NULL,0,0);
}

void launch_mixer () 
{
	char sys[100];
	sprintf(sys,"%s %c",opt_mixer,'&');
	system(sys);
}

void LoadPixbufs()
{
	GtkImage *tmp_image;
	int i;
	gchar *pixmap_path=g_build_filename(PACKAGE_DATA_DIR,"pixmaps",PACKAGE_NAME,NULL); 
	for (i=0;i<4;i++) {
		tmp_image=GTK_IMAGE(gtk_image_new_from_file(g_strconcat(pixmap_path,"/",tray_image_stocks[i],".png",NULL)));
		tray_pixbufs[i]=gtk_image_get_pixbuf (tmp_image);
	}
	g_free(pixmap_path);

}

void update_tray_image()
{
	gint vol = vol_get();
	gchar volchar[5]; 
	GdkPixbuf *tmp_pixbuf;
	if (!opt_gnome_icons)
	{	
		if (gtk_image_get_storage_type(tray_image) == GTK_IMAGE_PIXBUF) tmp_pixbuf=gtk_image_get_pixbuf (tray_image);
		if (vol>0 && vol<=33 && tmp_pixbuf!=tray_pixbufs[1])  gtk_image_set_from_pixbuf (tray_image,tray_pixbufs[1]);
    	if (vol>33 && vol<=66 && tmp_pixbuf!=tray_pixbufs[2] ) gtk_image_set_from_pixbuf (tray_image,tray_pixbufs[2]);
    	if (vol>66 && vol<=100 && tmp_pixbuf!=tray_pixbufs[3]) gtk_image_set_from_pixbuf (tray_image,tray_pixbufs[3]);
    	if (vol==0 && tmp_pixbuf!=tray_pixbufs[0]) gtk_image_set_from_pixbuf (tray_image,tray_pixbufs[0]);
	}
	else {
		gchar icon_name[50];
		if (gtk_image_get_storage_type(tray_image) == GTK_IMAGE_STOCK) gtk_image_get_icon_name(tray_image,&icon_name,NULL);
		if (vol>0 && vol<=33 && strcmp(icon_name,tray_image_stocks[1])!=0) gtk_image_set_from_icon_name (tray_image,tray_image_stocks[1],GTK_ICON_SIZE_BUTTON);
    	if (vol>33 && vol<=66 && strcmp(icon_name,tray_image_stocks[2])!=0) gtk_image_set_from_icon_name (tray_image,tray_image_stocks[2],GTK_ICON_SIZE_BUTTON);
   		if (vol>66 && vol<=100 && strcmp(icon_name,tray_image_stocks[3])!=0) gtk_image_set_from_icon_name (tray_image,tray_image_stocks[3],GTK_ICON_SIZE_BUTTON);
    	if (vol==0 && strcmp(icon_name,tray_image_stocks[0])!=0) gtk_image_set_from_icon_name (tray_image,tray_image_stocks[0],GTK_ICON_SIZE_BUTTON);
	}
	g_sprintf(volchar,"%i\%%",vol);
	if (opt_show_tooltip) gtk_widget_set_tooltip_text(GTK_WIDGET(tray_box),volchar);

}

void vol_window_show ()
{
	if (!GTK_IS_WINDOW(vwin)) vwin = GTK_WINDOW(gtk_window_new (GTK_WINDOW_TOPLEVEL));
	else {
		vol_window_close();	
		return;
	}
	
	gtk_container_set_border_width(GTK_CONTAINER(vwin),5);
	gtk_window_set_decorated (vwin,FALSE);
	gtk_window_set_position (vwin,GTK_WIN_POS_MOUSE);
	g_signal_connect(G_OBJECT(vwin),"focus-out-event",G_CALLBACK(vol_window_close),NULL);
	
	GtkVBox *vbox = GTK_VBOX(gtk_vbox_new (0,4));
	
	GtkButton *btn = GTK_BUTTON(gtk_button_new_with_label ("Mixer"));
	g_signal_connect(G_OBJECT(btn),"clicked",G_CALLBACK(launch_mixer),NULL);
	
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(btn),0,0,0);
	
	gtk_box_pack_start (GTK_BOX(vbox),gtk_hseparator_new(),0,0,0);
	
	GtkVScale *vscale = GTK_VSCALE(gtk_vscale_new_with_range (0,100,1));
	gtk_scale_set_digits (GTK_SCALE(vscale),0);
	gtk_scale_set_value_pos (GTK_SCALE(vscale),GTK_POS_BOTTOM);
	gtk_range_set_inverted (GTK_RANGE(vscale),TRUE);
	gtk_widget_set_size_request(GTK_WIDGET(vscale),0,120);

	gtk_range_set_value (GTK_RANGE(vscale),vol_get());
	g_signal_connect(vscale,"value-changed",G_CALLBACK(on_vol_value_change),NULL);
	
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(vscale),0,0,0);
	
	GtkLabel *label = GTK_LABEL(gtk_label_new("Master"));
	if (opt_channel == OPT_CHANNEL_MASTER) gtk_label_set_label (label,"Master");
	if (opt_channel == OPT_CHANNEL_PCM) gtk_label_set_label (label,"PCM");
	gtk_box_pack_start (GTK_BOX(vbox),GTK_WIDGET(label),0,0,0);
	
	gtk_container_add(GTK_CONTAINER(vwin),GTK_WIDGET(vbox));
	gtk_widget_show_all (GTK_WIDGET(vwin));
	gtk_window_set_focus (vwin,NULL);
}

void vol_window_close()
{
	gtk_widget_destroy(GTK_WIDGET(vwin));
}

void load_config ()
{
	gchar *config_filename;
	config_filename = g_build_filename(getenv("HOME"),".config",PACKAGE_NAME,"gvolwheel.conf",NULL);
	GKeyFile *config = g_key_file_new();
		
	if (g_key_file_load_from_file(config,config_filename,G_KEY_FILE_NONE,NULL) == FALSE) return;	
	opt_channel = g_key_file_get_integer(config,"Options","Channel",NULL);
	strcpy(opt_mixer,g_ascii_strdown(g_key_file_get_string(config,"Options","Mixer",NULL),-1));
	opt_incr = g_key_file_get_integer(config,"Options","Incr",NULL);
	opt_gnome_icons = g_key_file_get_boolean(config,"Options","Gnome_Icons",NULL);	
	opt_show_tooltip = g_key_file_get_boolean(config,"Options","Show_Tooltip",NULL);	

	g_key_file_free(config);
	g_free(config_filename);
	
}

void save_config()
{	
	gchar *config_filename;
	config_filename = g_build_filename(getenv("HOME"),".config",GETTEXT_PACKAGE,"gvolwheel.conf",NULL);
	GKeyFile *config = g_key_file_new();
	
	g_mkdir(strcat(getenv("HOME"),"/.config/gvolwheel"),0755);
	
	g_key_file_set_integer(config,"Options","Channel",opt_channel);	  
	g_key_file_set_string(config,"Options","Mixer",g_ascii_strdown(opt_mixer,-1));	  
	g_key_file_set_integer(config,"Options","Incr",opt_incr);	
	g_key_file_set_boolean(config,"Options","Gnome_Icons",opt_gnome_icons);	
	g_key_file_set_boolean(config,"Options","Show_Tooltip",opt_show_tooltip);	
	gchar *buffer = g_key_file_to_data(config, NULL, NULL);
	g_file_set_contents(config_filename, buffer, strlen(buffer), NULL);
	
	g_key_file_free(config);
	g_free(buffer);
	g_free(config_filename);
}
