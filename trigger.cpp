/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trigger.h"
#include "render.h"
#include "world.h"

using namespace std;

Trigger::Trigger(double get_x,double get_y,double get_w,double get_h,vector<target> get_targets,bool get_trigger_method,short get_user_type,bool get_repeating,short get_repeat_time,short get_render_trigger){
    x=get_x;
    y=get_y;

    w=get_w;
    h=get_h;

    render_trigger=get_render_trigger;

    repeat_time=get_repeat_time;

    targets=get_targets;

    trigger_method=get_trigger_method;

    user_type=get_user_type;

    repeating=get_repeating;

    frame=0;
    frame_counter=0;

    for(short n=0;n<5;n++){
        sprites_triggers[n].x=0;
        sprites_triggers[n].y=n*32;
        sprites_triggers[n].w=32;
        sprites_triggers[n].h=32;
    }
}

void Trigger::render(bool active_trigger){
    //If the triggers layer is currently being displayed.
    if(brush.layer_triggers){
        //If in camera bounds, render.
        if(x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            if(render_trigger>0){
                render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),64,160,sprite_sheet_triggers,&sprites_triggers[render_trigger-1]);
            }

            if(!active_trigger){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),w,h,0.5,1.0,0.0,0.0);
            }
            else{
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),w,h,0.5,0.0,1.0,0.0);
            }
        }
    }
}
