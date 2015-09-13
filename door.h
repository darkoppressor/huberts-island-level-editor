/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef door_h
#define door_h

#include <SDL.h>

class Door{
    private:
    //Editor only.
    SDL_Rect sprites_doors[4];
    SDL_Rect sprites_doors_global[12];
    SDL_Rect sprite_door_standard;

    public:
    Door(double get_x,double get_y,short get_type,short get_number,bool get_open,short get_identifier);

    //Render the sprite.
    void render(bool active_door);

    //The type of door.
    //0 = Local level door. (Affected by a trigger in this level.)
    //1 = Key door.
    //2 = Standard door.
    short type;

    //If type is 0 (i.e. this door is a local level door), this is the door number.
    //The door number determines what door sprite will be used for this door.
    short number;

    //If true, the door is open.
    //If false, the door is closed.
    bool open;

    short identifier;

    //The current coordinates of the platform.
    double x,y;

    //The dimensions of the platform.
    double w,h;
};

#endif
