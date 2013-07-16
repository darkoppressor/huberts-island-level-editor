/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef npc_h
#define npc_h

#include <SDL.h>

class Npc{
    private:
    SDL_Rect sprites_npcs[49];

    public:
    //
    Npc(double get_x,double get_y,short get_type);

    //Render the sprite.
    void render();

    //The current coordinates of the npc.
    double x,y;

    //The dimensions of the npc. These are used for collision detection.
    double w,h;

    //The type of NPC.
    short type;
};

#endif
