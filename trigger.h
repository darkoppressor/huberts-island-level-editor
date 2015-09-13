/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef trigger_h
#define trigger_h

#include <vector>
#include <SDL.h>

struct target{
    //The type of game object that this trigger targets.
    //0 = Moving platform.
    //1 = Door.
    //2 = Trap.
    short type;

    //The unique identifier of this target.
    short identifier;
};

class Trigger{
    private:
    //Editor only.
    SDL_Rect sprites_triggers[5];

    public:
    Trigger(double get_x,double get_y,double get_w,double get_h,std::vector<target> get_targets,bool get_trigger_method,short get_user_type,bool get_repeating,short get_repeat_time,short get_render_trigger);

    //Render the sprite.
    void render(bool active_trigger);

    //
    short render_trigger;

    //
    short repeat_time;

    //
    std::vector<target> targets;

    //What causes this trigger to change states.
    //For now, at least, only manual activation triggers will be rendered.
    //False = Manual activation.
    //True = Touch.
    bool trigger_method;

    //What kind of game objects can use this trigger.
    //0 = Player.
    //1 = Any npc.
    short user_type;

    //If true, the trigger will remain active after use.
    //If false, the trigger will become inactive after use.
    bool repeating;

    //Current coordinates.
    double x,y;

    //Dimensions.
    double w,h;

    //Animation frame and frame counter.
    short frame,frame_counter;
};

#endif
