/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "item.h"
#include "world.h"
#include "render.h"
#include "item_dimensions.h"

using namespace std;

Item::Item(double get_x,double get_y,int get_type,int get_goal_level_to_load,bool get_goal_secret){
    //Variables:

    type=get_type;

    goal_level_to_load=get_goal_level_to_load;

    goal_secret=get_goal_secret;

    //Starting coordinates must be given.
    x=get_x;
    y=get_y;

    //Dimensions are predetermined.
    w=ITEM_SIZE;
    h=ITEM_SIZE;

    sprites_items[ITEM_LEAF].x=0;
    sprites_items[ITEM_LEAF].y=0;
    sprites_items[ITEM_LEAF].w=16;
    sprites_items[ITEM_LEAF].h=16;

    sprites_items[ITEM_CHEESE].x=0;
    sprites_items[ITEM_CHEESE].y=16;
    sprites_items[ITEM_CHEESE].w=64;
    sprites_items[ITEM_CHEESE].h=64;

    sprites_items[ITEM_AMMO].x=0;
    sprites_items[ITEM_AMMO].y=80;
    sprites_items[ITEM_AMMO].w=32;
    sprites_items[ITEM_AMMO].h=32;

    sprites_items[ITEM_SPAWNPOINT].x=0;
    sprites_items[ITEM_SPAWNPOINT].y=0;
    sprites_items[ITEM_SPAWNPOINT].w=32;
    sprites_items[ITEM_SPAWNPOINT].h=32;

    sprites_items[ITEM_CHECKPOINT].x=32;
    sprites_items[ITEM_CHECKPOINT].y=0;
    sprites_items[ITEM_CHECKPOINT].w=32;
    sprites_items[ITEM_CHECKPOINT].h=96;

    sprites_items[ITEM_ENDPOINT].x=64;
    sprites_items[ITEM_ENDPOINT].y=0;
    sprites_items[ITEM_ENDPOINT].w=32;
    sprites_items[ITEM_ENDPOINT].h=96;

    sprites_items[ITEM_SWIMMING_GEAR].x=0;
    sprites_items[ITEM_SWIMMING_GEAR].y=80+32;
    sprites_items[ITEM_SWIMMING_GEAR].w=32;
    sprites_items[ITEM_SWIMMING_GEAR].h=32;

    sprites_items[ITEM_KEY_RED].x=0;
    sprites_items[ITEM_KEY_RED].y=80+32*2;
    sprites_items[ITEM_KEY_RED].w=32;
    sprites_items[ITEM_KEY_RED].h=32;

    sprites_items[ITEM_KEY_BLUE].x=0;
    sprites_items[ITEM_KEY_BLUE].y=80+32*3;
    sprites_items[ITEM_KEY_BLUE].w=32;
    sprites_items[ITEM_KEY_BLUE].h=32;

    sprites_items[ITEM_KEY_GREEN].x=0;
    sprites_items[ITEM_KEY_GREEN].y=80+32*4;
    sprites_items[ITEM_KEY_GREEN].w=32;
    sprites_items[ITEM_KEY_GREEN].h=32;

    sprites_items[ITEM_KEY_YELLOW].x=0;
    sprites_items[ITEM_KEY_YELLOW].y=80+32*5;
    sprites_items[ITEM_KEY_YELLOW].w=32;
    sprites_items[ITEM_KEY_YELLOW].h=32;

    sprites_items[ITEM_KEY_ORANGE].x=0;
    sprites_items[ITEM_KEY_ORANGE].y=80+32*6;
    sprites_items[ITEM_KEY_ORANGE].w=32;
    sprites_items[ITEM_KEY_ORANGE].h=32;

    sprites_items[ITEM_KEY_PURPLE].x=0;
    sprites_items[ITEM_KEY_PURPLE].y=80+32*7;
    sprites_items[ITEM_KEY_PURPLE].w=32;
    sprites_items[ITEM_KEY_PURPLE].h=32;

    sprites_items[ITEM_TOWEL].x=0;
    sprites_items[ITEM_TOWEL].y=80+32*8;
    sprites_items[ITEM_TOWEL].w=32;
    sprites_items[ITEM_TOWEL].h=32;

    sprites_items[ITEM_SINK].x=0;
    sprites_items[ITEM_SINK].y=80+32*9;
    sprites_items[ITEM_SINK].w=32;
    sprites_items[ITEM_SINK].h=32;

    sprites_items[ITEM_J_BALLOON].x=0;
    sprites_items[ITEM_J_BALLOON].y=80+32*10;
    sprites_items[ITEM_J_BALLOON].w=32;
    sprites_items[ITEM_J_BALLOON].h=32;

    sprites_items[ITEM_AMMO_BARREL].x=0;
    sprites_items[ITEM_AMMO_BARREL].y=80+32*11;
    sprites_items[ITEM_AMMO_BARREL].w=34;
    sprites_items[ITEM_AMMO_BARREL].h=48;

    sprites_items[ITEM_CANDY].x=0;
    sprites_items[ITEM_CANDY].y=80+32*11+48;
    sprites_items[ITEM_CANDY].w=16;
    sprites_items[ITEM_CANDY].h=16;

    sprites_items[ITEM_KEY_GRAY].x=0;
    sprites_items[ITEM_KEY_GRAY].y=80+32*11+48+16;
    sprites_items[ITEM_KEY_GRAY].w=32;
    sprites_items[ITEM_KEY_GRAY].h=32;

    sprites_items[ITEM_KEY_BROWN].x=0;
    sprites_items[ITEM_KEY_BROWN].y=80+32*11+48+16+32;
    sprites_items[ITEM_KEY_BROWN].w=32;
    sprites_items[ITEM_KEY_BROWN].h=32;

    sprites_items[ITEM_KEY_BLACK].x=0;
    sprites_items[ITEM_KEY_BLACK].y=80+32*11+48+16+32*2;
    sprites_items[ITEM_KEY_BLACK].w=32;
    sprites_items[ITEM_KEY_BLACK].h=32;

    sprites_items[ITEM_KEY_PINK].x=0;
    sprites_items[ITEM_KEY_PINK].y=80+32*11+48+16+32*3;
    sprites_items[ITEM_KEY_PINK].w=32;
    sprites_items[ITEM_KEY_PINK].h=32;

    sprites_items[ITEM_KEY_CYAN].x=0;
    sprites_items[ITEM_KEY_CYAN].y=80+32*11+48+16+32*4;
    sprites_items[ITEM_KEY_CYAN].w=32;
    sprites_items[ITEM_KEY_CYAN].h=32;

    sprites_items[ITEM_SUIT_DEADLY_WATER].x=0;
    sprites_items[ITEM_SUIT_DEADLY_WATER].y=80+32*11+48+16+32*5;
    sprites_items[ITEM_SUIT_DEADLY_WATER].w=32;
    sprites_items[ITEM_SUIT_DEADLY_WATER].h=32;

    sprites_items[ITEM_SUIT_SHARP].x=0;
    sprites_items[ITEM_SUIT_SHARP].y=80+32*11+48+16+32*6;
    sprites_items[ITEM_SUIT_SHARP].w=32;
    sprites_items[ITEM_SUIT_SHARP].h=32;

    sprites_items[ITEM_SUIT_BANANA].x=0;
    sprites_items[ITEM_SUIT_BANANA].y=80+32*11+48+16+32*7;
    sprites_items[ITEM_SUIT_BANANA].w=32;
    sprites_items[ITEM_SUIT_BANANA].h=32;

    sprites_items[ITEM_SHOT_HOMING].x=0;
    sprites_items[ITEM_SHOT_HOMING].y=80+32*11+48+16+32*8;
    sprites_items[ITEM_SHOT_HOMING].w=32;
    sprites_items[ITEM_SHOT_HOMING].h=32;

    sprites_items[ITEM_TRANSLATOR].x=0;
    sprites_items[ITEM_TRANSLATOR].y=80+32*11+48+16+32*9;
    sprites_items[ITEM_TRANSLATOR].w=32;
    sprites_items[ITEM_TRANSLATOR].h=32;

    sprites_items[ITEM_J_WING].x=0;
    sprites_items[ITEM_J_WING].y=80+32*11+48+16+32*10;
    sprites_items[ITEM_J_WING].w=32;
    sprites_items[ITEM_J_WING].h=32;
}

void Item::render(bool active){
    //If the items layer is currently being displayed.
    if(brush.layer_items){
        //If the item is in camera bounds, render the item.
        if(x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            if(type>=ITEM_LEAF && type<=ITEM_AMMO){
                if(type==ITEM_LEAF){
                    render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.25,1.0,0.0,0.0);
                }
                else if(type==ITEM_CHEESE){
                    render_rectangle(x-brush.camera.x,y-brush.camera.y,64,64,0.25,1.0,0.0,0.0);
                }
                else if(type==ITEM_AMMO){
                    render_rectangle(x-brush.camera.x,y-brush.camera.y,32,32,0.25,1.0,0.0,0.0);
                }
                render_sprite(x-brush.camera.x,y-brush.camera.y,128,848,sprite_sheet_items,&sprites_items[type]);
                render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.50,1.0,0.0,0.0);
            }
            else if(type>=ITEM_SPAWNPOINT && type<=ITEM_ENDPOINT){
                render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.50,1.0,0.0,0.0);
                /**if(type==ITEM_SPAWNPOINT){
                    render_rectangle(x-brush.camera.x,y-brush.camera.y,32,32,0.25,1.0,0.0,0.0);
                }
                else{*/
                    render_rectangle(x-brush.camera.x,y-brush.camera.y,sprites_items[type].w,sprites_items[type].h,0.25,1.0,0.0,0.0);
                ///}
                if(type!=ITEM_ENDPOINT || (type==ITEM_ENDPOINT && !goal_secret)){
                    render_sprite(x-brush.camera.x,y-brush.camera.y,128,96,sprite_sheet_level_items,&sprites_items[type]);
                }
                else{
                    SDL_Rect secret_goal;
                    secret_goal.x=3*32;
                    secret_goal.y=0;
                    secret_goal.w=32;
                    secret_goal.h=96;
                    render_sprite(x-brush.camera.x,y-brush.camera.y,128,96,sprite_sheet_level_items,&secret_goal);
                }
            }
            else if(type>=ITEM_SWIMMING_GEAR && type<=ITEM_END && type!=ITEM_CANDY){
                render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.25,1.0,0.0,0.0);
                render_sprite(x-brush.camera.x,y-brush.camera.y,128,848,sprite_sheet_items,&sprites_items[type]);
                render_rectangle(x-brush.camera.x,y-brush.camera.y,32,32,0.50,1.0,0.0,0.0);
            }
            else if(type==ITEM_CANDY){
                render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.25,1.0,0.0,0.0);
                render_sprite(x-brush.camera.x,y-brush.camera.y,128,848,sprite_sheet_items,&sprites_items[type]);
                ///render_rectangle(x-brush.camera.x,y-brush.camera.y,32,32,0.50,1.0,0.0,0.0);
            }
            else if(type<=-1 || (brush.WORLD_MAP && type==0)){
                render_rectangle(x-brush.camera.x,y-brush.camera.y,32,32,0.25,1.0,0.0,0.0);
                render_rectangle(x-brush.camera.x,y-brush.camera.y,16,16,0.50,1.0,0.0,0.0);
                ss.clear();ss.str("");ss<<fabs(type);msg=ss.str();
                font.show(x-brush.camera.x,y-brush.camera.y,msg,COLOR_WHITE);
            }

            if(active){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_items[type].w,sprites_items[type].h,0.50,0.0,1.0,0.0);
            }
        }
    }
}
