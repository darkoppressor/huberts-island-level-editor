/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "door.h"
#include "render.h"
#include "world.h"

using namespace std;

Door::Door(double get_x,double get_y,short get_type,short get_number,bool get_open,short get_identifier){
    x=get_x;
    y=get_y;

    w=DOOR_SIZE;
    h=DOOR_SIZE;

    type=get_type;

    number=get_number;

    open=get_open;

    identifier=get_identifier;

    for(short i=0;i<4;i++){
        sprites_doors[i].x=32;
        sprites_doors[i].y=i*96;
        sprites_doors[i].w=32;
        sprites_doors[i].h=96;
    }

    for(short i=0;i<12;i++){
        sprites_doors_global[i].x=288+(32*i);
        sprites_doors_global[i].y=0;
        sprites_doors_global[i].w=32;
        sprites_doors_global[i].h=96;
    }

    sprite_door_standard.x=0;
    sprite_door_standard.y=0;
    sprite_door_standard.w=32;
    sprite_door_standard.h=96;
}

void Door::render(bool active_door){
    //If the moving platforms layer is currently being displayed.
    if(brush.layer_doors){
        //If the door is in camera bounds, render it.
        if(x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            //If the door is a local level door.
            if(type==0 && number<4){
                render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),672,384,sprite_sheet_doors,&sprites_doors[number]);
            }
            else if(type==1){
                render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),672,384,sprite_sheet_doors,&sprites_doors_global[number]);
            }
            else if(type==2){
                render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,384,sprite_sheet_door_standard,&sprite_door_standard);
            }

            if(!active_door){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),DOOR_SIZE,DOOR_SIZE,0.50,1.0,0.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_doors[0].w,sprites_doors[0].h,0.05,1.0,0.0,0.0);
            }
            else{
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),DOOR_SIZE,DOOR_SIZE,0.50,0.0,1.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_doors[0].w,sprites_doors[0].h,0.05,0.0,1.0,0.0);
            }
        }

        //If a trigger is selected.
        if(brush.current_trigger!=-1){
            for(int i=0;i<vector_triggers[brush.current_trigger].targets.size();i++){
                if(vector_triggers[brush.current_trigger].targets[i].type==1 && vector_triggers[brush.current_trigger].targets[i].identifier==identifier){
                    render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_doors[0].w,sprites_doors[0].h,0.10,0.0,0.0,1.0);
                    break;
                }
            }
        }
    }
}
