/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef item_h
#define item_h

#include "enumerations.h"

#include <SDL.h>

class Item{
    private:
    //This holds the single frame for the item sprite for now. When animation is implemented, it will change.
    SDL_Rect sprites_items[ITEM_END];

    public:
    //The contructor takes starting coordinates and a movement direction.
    Item(double get_x,double get_y,int get_type,int get_goal_level_to_load,bool get_goal_secret);

    //Render the sprite.
    void render(bool active);

    //Negative numbers: levels.
    //So -1 is level 1, -2 is level 2, etc.
    //0 = None.
    //1 = Leaf.
    //2 = Cheese.
    //3 = Ammo box.
    //4 = Spawn point.
    //5 = Checkpoint.
    //6 = End point.
    //7 = Swimming gear.
    //8 = Red key.
    //9 = Blue key.
    //10 = Green key.
    //11 = Yellow key.
    int type;

    //The level to load when this goal item is reached.
    //Only used if the item is a goal item, obviously.
    int goal_level_to_load;

    //If true, the goal is a secret goal.
    //If false, the goal is a standard goal.
    bool goal_secret;

    //The current coordinates of the item.
    double x,y;

    //The dimensions of the item. These are used for collision detection.
    double w,h;
};

#endif
