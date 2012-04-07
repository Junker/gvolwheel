#include <glib.h>


#define OPT_CHANNEL_MASTER	0
#define OPT_CHANNEL_PCM		1 

gchar opt_mixer[100];
guint opt_channel;
guint opt_incr;
gboolean opt_gnome_icons;
gboolean opt_show_tooltip;

void load_config();
void save_config();