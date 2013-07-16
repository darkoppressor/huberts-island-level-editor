/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef booster_h
#define booster_h

#include <SDL.h>

class Booster{
    private:
    SDL_Rect sprite_booster[1];

    public:
    Booster(double get_x,double get_y,double get_speed,short get_direction);

    void render(bool active);

    double x,y;

    //The speed the booster sets/adds to the actor affected.
    double speed;

    short direction;
};

#endif
