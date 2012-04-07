/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * volume.c
 * Copyright (C) Dmitry Kosenkov 2011 <junker@front.ru>
 * 
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "volume.h"
#include "conf.h"
#include "config.h"

#ifdef BACKEND_ALSA
	#include "alsa.h"
#else
	#include "oss.h"
#endif

int vol_init()
{
	return vol_backend_init();
}

void vol_up () 
{
	vol_set(vol_get() + opt_incr);
}

void vol_down () 
{
	vol_set(vol_get() - opt_incr);
}

void vol_mute () 
{
	vol_set(0);
}

void vol_set (int value)
{
	if (value < 0) value=0;
	vol_backend_set(opt_channel, value);
}

int vol_get ()
{
	return vol_backend_get(opt_channel);
}