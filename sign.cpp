/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sign.h"
#include "item_dimensions.h"
#include "world.h"
#include "render.h"

using namespace std;

Sign::Sign(double get_x,double get_y,string get_message,short get_type,short get_font_type){
    x=get_x;
    y=get_y;

    message=get_message;

    type=get_type;

    font_type=get_font_type;

    w=SIGN_SIZE;
    h=SIGN_SIZE;

    sprite_sign[0].x=0;
    sprite_sign[0].y=0;
    sprite_sign[0].w=SIGN_SIZE;
    sprite_sign[0].h=SIGN_SIZE;

    sprite_sign[1].x=SIGN_SIZE;
    sprite_sign[1].y=0;
    sprite_sign[1].w=SIGN_SIZE;
    sprite_sign[1].h=SIGN_SIZE;
}

void Sign::render(bool active){
    //If the signs layer is currently being displayed.
    if(brush.layer_signs){
        //If the sign is in camera bounds, render the sign.
        if(x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            render_sprite(x-brush.camera.x,y-brush.camera.y,64,32,sprite_sheet_signs,&sprite_sign[type]);

            if(active){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),SIGN_SIZE,SIGN_SIZE,0.50,0.0,1.0,0.0);
            }
        }
    }
}
