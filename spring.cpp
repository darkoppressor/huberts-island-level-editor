/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "spring.h"
#include "world.h"
#include "render.h"

using namespace std;

Spring::Spring(double get_x,double get_y,double get_speed,short get_direction){
    x=get_x;
    y=get_y;
    speed=get_speed;
    direction=get_direction;

    sprite_spring[0].x=0;
    sprite_spring[0].y=0;
    sprite_spring[0].w=SPRING_SIZE;
    sprite_spring[0].h=SPRING_SIZE;
}

void Spring::render(bool active){
    //If the springs layer is currently being displayed.
    if(brush.layer_signs){
        if(x>=brush.camera.x-SPRING_SIZE && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-SPRING_SIZE && y<=brush.camera.y+brush.camera.h){
            double angle=0.0;

            if(direction==LEFT){
                angle=90.0;
            }
            else if(direction==UP){
                angle=0.0;
            }
            else if(direction==RIGHT){
                angle=270.0;
            }
            else if(direction==DOWN){
                angle=180.0;
            }
            else if(direction==LEFT_UP){
                angle=45.0;
            }
            else if(direction==LEFT_DOWN){
                angle=135.0;
            }
            else if(direction==RIGHT_UP){
                angle=315.0;
            }
            else if(direction==RIGHT_DOWN){
                angle=225.0;
            }

            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,32,sprite_sheet_spring,&sprite_spring[0],1.0,false,angle);

            if(active){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),SPRING_SIZE,SPRING_SIZE,0.50,0.0,1.0,0.0);
            }
        }
    }
}
