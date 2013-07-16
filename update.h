/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef update_h
#define update_h

#include <SDL.h>

void input();

void movement();

void events();

void animation();

void camera();

void render(int frame_rate,double ms_per_frame);

#endif
