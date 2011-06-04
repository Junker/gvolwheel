#include <gtk/gtk.h>

void on_mconfig_click (GtkMenuItem *menuitem);
void on_vol_value_change (GtkRange *range,GtkScrollType step,gpointer user_data); 
void on_mabout_click (GtkMenuItem *menuitem);
void on_timer(gpointer data);