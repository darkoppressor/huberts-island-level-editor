/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef trap_h
#define trap_h

#include <SDL.h>

class Trap{
    private:
    SDL_Rect sprites_traps[19];

    public:

    Trap(double get_x,double get_y,short get_type,bool get_active,short get_identifier);

    void render(bool active_trap);

    //The type of trap.
    //See trap_data.h for a full list of trap types.
    short type;

    //If true, the trap acts according to its danger_type and movement_type.
    //If false, the trap is harmless and does nothing.
    bool active;

    //Coordinates.
    double x,y;

    //Dimensions.
    double w,h;

    short identifier;
};

#endif
