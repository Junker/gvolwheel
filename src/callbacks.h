#include <gtk/gtk.h>

void on_vol_value_change (GtkRange *range,GtkScrollType step,gpointer user_data); 
void on_mabout_click (GtkMenuItem *menuitem);
void on_mconfig_click (GtkMenuItem *menuitem);
void on_config_window_close(GtkButton *button,gpointer user_data);
void on_config_window_save(GtkButton *button,gpointer user_data); 
void on_tray_icon_scroll (GtkWidget *widget,GdkEventScroll *event,gpointer user_data) ;
void on_tray_icon_click(GtkWidget *widget,GdkEventButton *event,gpointer user_data);
void on_tray_icon_press(GtkWidget *widget,GdkEventButton *event,gpointer user_data);
gboolean on_timer(gpointer data);