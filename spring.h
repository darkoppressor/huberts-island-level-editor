/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef spring_h
#define spring_h

#include <SDL.h>

class Spring{
    private:
    SDL_Rect sprite_spring[1];

    public:
    Spring(double get_x,double get_y,double get_speed,short get_direction);

    void render(bool active);

    double x,y;

    //The speed the spring sets/adds to the actor affected.
    double speed;

    short direction;
};

#endif
