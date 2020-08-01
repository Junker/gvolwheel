#include <glib.h>


#define OPT_CHANNEL_MASTER	0
#define OPT_CHANNEL_PCM		1

extern gchar opt_mixer[100];
extern guint opt_channel;
extern guint opt_incr;
extern gboolean opt_gnome_icons;
extern gboolean opt_show_tooltip;

void load_config();
void save_config();
