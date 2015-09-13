/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trap.h"
#include "render.h"
#include "world.h"
#include "trap_dimensions.h"

using namespace std;

Trap::Trap(double get_x,double get_y,short get_type,bool get_active,short get_identifier){
    x=get_x;
    y=get_y;

    w=TRAP_SIZE;
    h=TRAP_SIZE;

    type=get_type;

    active=get_active;

    identifier=get_identifier;

    sprites_traps[1].x=0;
    sprites_traps[1].y=0;
    sprites_traps[1].w=16;
    sprites_traps[1].h=32;

    sprites_traps[2].x=16;
    sprites_traps[2].y=0;
    sprites_traps[2].w=32;
    sprites_traps[2].h=32;

    sprites_traps[3].x=48;
    sprites_traps[3].y=0;
    sprites_traps[3].w=16;
    sprites_traps[3].h=32;

    sprites_traps[4].x=64;
    sprites_traps[4].y=0;
    sprites_traps[4].w=41;
    sprites_traps[4].h=23;

    sprites_traps[5].x=128;
    sprites_traps[5].y=0;
    sprites_traps[5].w=32;
    sprites_traps[5].h=192;

    sprites_traps[6].x=160;
    sprites_traps[6].y=0;
    sprites_traps[6].w=64;
    sprites_traps[6].h=32;

    sprites_traps[7].x=220;
    sprites_traps[7].y=0;
    sprites_traps[7].w=32;
    sprites_traps[7].h=32;

    sprites_traps[8].x=244;
    sprites_traps[8].y=0;
    sprites_traps[8].w=32;
    sprites_traps[8].h=32;

    sprites_traps[9].x=244+32;
    sprites_traps[9].y=0;
    sprites_traps[9].w=32;
    sprites_traps[9].h=32;

    sprites_traps[10].x=244+32*2;
    sprites_traps[10].y=0;
    sprites_traps[10].w=32;
    sprites_traps[10].h=32;

    sprites_traps[11].x=244+32*3;
    sprites_traps[11].y=0;
    sprites_traps[11].w=32;
    sprites_traps[11].h=32;

    sprites_traps[12].x=244+32*4;
    sprites_traps[12].y=0;
    sprites_traps[12].w=32;
    sprites_traps[12].h=32;

    sprites_traps[13].x=244+32*5;
    sprites_traps[13].y=0;
    sprites_traps[13].w=32;
    sprites_traps[13].h=32;

    sprites_traps[14].x=0;
    sprites_traps[14].y=0;
    sprites_traps[14].w=0;
    sprites_traps[14].h=0;

    sprites_traps[15].x=0;
    sprites_traps[15].y=0;
    sprites_traps[15].w=0;
    sprites_traps[15].h=0;

    sprites_traps[16].x=244+32*6;
    sprites_traps[16].y=0;
    sprites_traps[16].w=32;
    sprites_traps[16].h=64;

    sprites_traps[17].x=244+32*7;
    sprites_traps[17].y=0;
    sprites_traps[17].w=32;
    sprites_traps[17].h=32;

    sprites_traps[18].x=244+32*8;
    sprites_traps[18].y=0;
    sprites_traps[18].w=32;
    sprites_traps[18].h=32;
}

void Trap::render(bool active_trap){
    //If the traps layer is currently being displayed.
    if(brush.layer_traps){
        //If in camera bounds, render.
        if(type!=0 && x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),768,384,sprite_sheet_traps,&sprites_traps[type]);

            if(!active_trap){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),TRAP_SIZE,TRAP_SIZE,0.50,1.0,0.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_traps[type].w,sprites_traps[type].h,0.25,1.0,0.0,0.0);
            }
            else{
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),TRAP_SIZE,TRAP_SIZE,0.50,0.0,1.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_traps[type].w,sprites_traps[type].h,0.25,0.0,1.0,0.0);
            }

            //If a trigger is selected.
            if(brush.current_trigger!=-1){
                for(int i=0;i<vector_triggers[brush.current_trigger].targets.size();i++){
                    if(vector_triggers[brush.current_trigger].targets[i].type==2 && vector_triggers[brush.current_trigger].targets[i].identifier==identifier){
                        render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_traps[type].w,sprites_traps[type].h,0.30,0.0,0.0,1.0);
                        break;
                    }
                }
            }
        }
    }
}
