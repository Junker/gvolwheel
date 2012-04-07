/*
 * oss.c
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

#include <linux/soundcard.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "oss.h"

static char *default_device = "/dev/mixer"; 

int mixer_fd;

int vol_backend_init (char *device)
{
	mixer_fd = open (device ? device : default_device, R_OK+W_OK, 0);
  	if (mixer_fd < 0) return 0;
	else return 1;

}

int vol_backend_get(int mixer)
{
	if (mixer == 0)
		ioctl(mixer_fd, MIXER_READ(SOUND_MIXER_VOLUME), &vol);
	else if (mixer == 1)	
		ioctl(mixer_fd, MIXER_READ(SOUND_MIXER_PCM), &vol);
	return(vol.left);
}

void vol_backend_set(int mixer, int value)
{
	vol.left=vol.right = value;
	if (mixer == 0)	
		ioctl(mixer_fd, MIXER_WRITE(SOUND_MIXER_VOLUME), &vol);
	else if (mixer == 1)	
		ioctl(mixer_fd, MIXER_WRITE(SOUND_MIXER_PCM), &vol);
}