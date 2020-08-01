/*
 * alsa.c
 *
 * Copyright (C) 2012 - Dmitry Kosenkov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <alsa/asoundlib.h>

#include "alsa.h"

static char *default_device = "default";

static snd_mixer_t * mixer_id = NULL;
snd_mixer_selem_id_t *sid;

int vol_backend_init (char *device)
{
	snd_mixer_open(&mixer_id, 0);
	snd_mixer_attach(mixer_id, device ? device : default_device);
	snd_mixer_selem_register(mixer_id, NULL, NULL);
	snd_mixer_load(mixer_id);

	snd_mixer_selem_id_malloc(&sid);

	snd_mixer_selem_id_set_name(sid, "Master");
	snd_mixer_elem_t* elem = snd_mixer_find_selem(mixer_id, sid);
	if (!elem) return 0;

	snd_mixer_selem_id_set_name(sid, "PCM");
	elem = snd_mixer_find_selem(mixer_id, sid);
	if (!elem) return 0;

	return 1;
}

int vol_backend_get(int mixer)
{
	snd_mixer_handle_events(mixer_id);
	snd_mixer_selem_id_set_name(sid, mixer == 0 ? "Master" : "PCM");
	snd_mixer_elem_t* elem = snd_mixer_find_selem(mixer_id, sid);

	long min, max, vol;
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	snd_mixer_selem_get_playback_volume(elem, 0, &vol);

	return 100 * vol / max;
}

void vol_backend_set(int mixer, int value)
{
	snd_mixer_selem_id_set_name(sid, mixer == 0 ? "Master" : "PCM");
	snd_mixer_elem_t* elem = snd_mixer_find_selem(mixer_id, sid);

	long min, max;
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	snd_mixer_selem_set_playback_volume_all(elem, max * value / 100);
}
