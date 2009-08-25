#include <gtk/gtk.h>

typedef struct stereovolume
{
  unsigned char left;
  unsigned char right;
} StereoVolume;


void vol_up ();
void vol_down ();
void vol_mute ();
void vol_set (int value);
int vol_get ();
void tray_icon_menu_show();
void launch_mixer ();
void LoadPixbufs();
void update_tray_image();
void vol_window_show ();
void vol_window_close();
void load_config();
void save_config();
