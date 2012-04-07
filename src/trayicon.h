#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <glib/gi18n-lib.h>

GtkStatusIcon *tray_icon; 
GdkPixbuf *tray_pixbufs[4]; 

GtkStatusIcon* create_tray_icon();
gboolean tray_icon_recreate(gpointer data);
void on_tray_icon_destroyed(GtkWidget *widget, void *data);
void on_tray_icon_scroll (GtkWidget *widget, GdkEventScroll *event, gpointer user_data);
void on_tray_icon_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void on_tray_icon_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void update_tray_image();
void load_pixbufs();