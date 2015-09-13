/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "booster.h"
#include "world.h"
#include "render.h"

using namespace std;

Booster::Booster(double get_x,double get_y,double get_speed,short get_direction){
    x=get_x;
    y=get_y;
    speed=get_speed;
    direction=get_direction;

    sprite_booster[0].x=0;
    sprite_booster[0].y=0;
    sprite_booster[0].w=64;
    sprite_booster[0].h=32;
}

void Booster::render(bool active){
    //If the boosters layer is currently being displayed.
    if(brush.layer_boosters){
        if(x>=brush.camera.x-BOOSTER_SIZE && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-BOOSTER_SIZE && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),64,32,sprite_sheet_booster,&sprite_booster[0]);

            if(active){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),64,32,0.50,0.0,1.0,0.0);
            }
        }
    }
}
