/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "update.h"
#include "world.h"
#include "quit.h"
#include "render.h"
#include "collision.h"
#include "sprite_selection_dialog.h"

#include <SDL.h>

using namespace std;

//Gather input.
void input(){
    //Check to see if the player has X'ed out of the game. If so, run quit_game().
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT){
            quit_game();
            break;
        }

        if(brush.layer_current==1){
            if(event.type==SDL_MOUSEBUTTONDOWN){
                if(event.button.button==SDL_BUTTON_WHEELUP){
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        brush.item_current++;
                        if(brush.item_current>ITEM_END-1){
                            brush.item_current=0;
                        }
                    }
                    //If we are editing the world map.
                    else{
                        brush.item_current--;
                        if(brush.item_current<-99){
                            brush.item_current=-99;
                        }
                        if(brush.item_current>0){
                            brush.item_current=0;
                        }
                    }
                }
                else if(event.button.button==SDL_BUTTON_WHEELDOWN){
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        brush.item_current--;
                        if(brush.item_current<0){
                            brush.item_current=ITEM_END-1;
                        }
                    }
                    //If we are editing the world map.
                    else{
                        brush.item_current++;
                        if(brush.item_current>0){
                            brush.item_current=4;
                        }
                    }
                }
            }
        }

        else if(brush.layer_current==3){
            if(event.type==SDL_MOUSEBUTTONDOWN){
                if(event.button.button==SDL_BUTTON_WHEELUP){
                    brush.npc_current++;
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        if(brush.npc_current>48){
                            brush.npc_current=0;
                        }
                    }
                }
                else if(event.button.button==SDL_BUTTON_WHEELDOWN){
                    brush.npc_current--;
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        if(brush.npc_current<0){
                            brush.npc_current=48;
                        }
                    }
                }
            }
        }

        else if(brush.layer_current==7){
            if(event.type==SDL_MOUSEBUTTONDOWN){
                if(event.button.button==SDL_BUTTON_WHEELUP){
                    brush.trap_current++;
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        if(brush.trap_current>18){
                            brush.trap_current=0;
                        }
                    }
                }
                else if(event.button.button==SDL_BUTTON_WHEELDOWN){
                    brush.trap_current--;
                    //If we are editing a level.
                    if(!brush.WORLD_MAP){
                        if(brush.trap_current<0){
                            brush.trap_current=18;
                        }
                    }
                }
            }
        }

        if(brush.edit_input==0){
            int mouse_x=0,mouse_y=0;
            if(event.type==SDL_MOUSEBUTTONUP){
                if(event.button.button==SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mouse_x,&mouse_y);/**Store the mouse's current coordinates in x and y.*/

                    //Adjust the mouse's position to make clicking work better.
                    int brush_size=0;

                    //Terrain tiles.
                    //Background tiles.
                    //Triggers.
                    //Doors.
                    //Signs
                    //Springs
                    //Boosters
                    if(brush.layer_current==0 || brush.layer_current==2 || brush.layer_current==5 || brush.layer_current==6 || brush.layer_current==8 || brush.layer_current==9 || brush.layer_current==10){
                        brush_size=32;
                    }
                    //Items.
                    //Npcs.
                    //Moving platforms.
                    //Traps.
                    else if(brush.layer_current==1 || brush.layer_current==3 || brush.layer_current==4 || brush.layer_current==7){
                        brush_size=16;
                    }

                    brush.x=mouse_x+brush.camera.x;
                    brush.y=mouse_y+brush.camera.y;
                    brush.x=(int)(brush.x/brush_size)*brush_size;
                    brush.y=(int)(brush.y/brush_size)*brush_size;
                }
            }
        }

        //If we are selecting a sprite, check for the user clicking on a sprite.
        if(brush.edit_input==4){
            int mouse_x=0,mouse_y=0;
            if(event.type==SDL_MOUSEBUTTONUP){
                if(event.button.button==SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mouse_x,&mouse_y);/**Store the mouse's current coordinates in x and y.*/
                    //Not the world map, editing terrain tiles.
                    if(!brush.WORLD_MAP && brush.layer_current==0){
                        for(int i=0;i<TILE_SPRITES;i++){
                            short row=set_tile_row(i);
                            short column=set_tile_column(i);

                            if(collision_check(mouse_x,mouse_y,2,2,(34*column),285+(row*63),32,32)){
                                brush.edit_sprite=i;
                                brush.edit_input++;
                            }
                        }
                    }
                    //Not the world map, editing background tiles.
                    if(!brush.WORLD_MAP && brush.layer_current==2){
                        for(int i=0;i<BACKGROUND_TILES;i++){
                            short row=set_tile_row(i);
                            short column=set_tile_column(i);

                            if(collision_check(mouse_x,mouse_y,2,2,(34*column),285+(row*63),32,32)){
                                brush.edit_sprite=i;
                                brush.edit_input++;
                            }
                        }
                    }
                    //World map.
                    else if(brush.WORLD_MAP){
                        for(int i=0;i<WORLDMAP_TILES;i++){
                            short row=set_tile_row(i);
                            short column=set_tile_column(i);

                            if(collision_check(mouse_x,mouse_y,2,2,(34*column),285+(row*63),32,32)){
                                brush.edit_sprite=i;
                                brush.edit_input++;
                            }
                        }
                    }
                }
            }
        }

        //If we are selecting a sprite, check for the user typing in the number of the sprite.
        if(brush.edit_input==4){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<2){/**If the string is less than the maximum size.*/
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){/**If the key is a number.*/
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a moving platforms's move_speed, check for the user typing in the number.
        if(brush.edit_input==1){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<7){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if(((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')) || event.key.keysym.unicode==(Uint16)'.'){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a spring's speed, check for the user typing in the number.
        if(brush.edit_input==22){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<7){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if(((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')) || event.key.keysym.unicode==(Uint16)'.'){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a booster's speed, check for the user typing in the number.
        if(brush.edit_input==24){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<7){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if(((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')) || event.key.keysym.unicode==(Uint16)'.'){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting an item's goal_level_to_load, check for the user typing in the number.
        if(brush.edit_input==19){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<7){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if(event.key.keysym.unicode>=(Uint16)'0' && event.key.keysym.unicode<=(Uint16)'9'){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a trigger's repeat_time, check for the user typing in the number.
        if(brush.edit_input==13){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<4){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are inputing dimensions for a new level, check for the user typing in the number.
        if(brush.edit_input==3){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<8){/**If the string is less than the maximum size.*/
                    //If the key is a number.
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are inputing dimensions for level resize, check for the user typing in the number.
        if(brush.edit_input==21){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<8){/**If the string is less than the maximum size.*/
                    //If the key is a number.
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a tile's slope, check for the user typing in the number.
        if(brush.edit_input==11){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<3){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a tile's height mask, check for the user typing in the number.
        if(brush.edit_input==15){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                if(brush.edit_string.length()<3){/**If the string is less than the maximum size.*/
                    //If the key is a number or a decimal.
                    if((event.key.keysym.unicode>=(Uint16)'0') && (event.key.keysym.unicode<=(Uint16)'9')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                }

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }

        //If we are setting a sign's message, check for the user typing in the message.
        if(brush.edit_input==17){
            SDL_EnableKeyRepeat(250,50);
            switch(event.type){
                case SDL_KEYDOWN:
                ///if(brush.edit_string.length()<3){/**If the string is less than the maximum size.*/
                    if(event.key.keysym.sym==SDLK_BACKSPACE && brush.edit_string.length()!=0){
                        brush.edit_string.erase(brush.edit_string.length()-1);
                    }
                    //If the key is a number or a decimal.
                    if((event.key.keysym.unicode>=(Uint16)' ') && (event.key.keysym.unicode<=(Uint16)'~')){
                        brush.edit_string+=(char)event.key.keysym.unicode;
                    }
                ///}

                if(event.key.keysym.sym==SDLK_F10){
                    quit_game();
                }
                break;
                case SDL_QUIT:
                quit_game();
                break;
            }
            SDL_EnableKeyRepeat(1,0);
        }
    }

    //Gather input from the brush.
    brush.handle_input();
}

//Move things that need to be moved.
void movement(){
    //Move the brush.
    brush.move();
}

//Handle events.
void events(){
}

//Handle camera movement.
void camera(){
    //Handle camera movements for the brush.
    brush.set_camera();
}

//Render everything to the screen.
void render(int frame_rate, double ms_per_frame){
    //Clear the screen, filling it with the clear color set in Window.init().
    glClear(GL_COLOR_BUFFER_BIT);

    brush.render_background();

    //Set the player's current tile position.
    int camera_current_x=(int)((int)brush.camera.x/32);
    int camera_current_y=(int)((int)brush.camera.y/32);

    //Check all tiles in a rectangle around the player.
    int check_x_start=camera_current_x;
    int check_x_end=camera_current_x+(int)((int)brush.camera.w/32)+1;
    int check_y_start=camera_current_y;
    int check_y_end=camera_current_y+(int)((int)brush.camera.h/32)+1;

    //Render each onscreen background and terrain tile.
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/32)-1 && int_y>=0 && int_y<=(level.level_y/32)-1){
                level.tile_background_array[int_x][int_y].render();
                level.tile_array[int_x][int_y].render();
            }
        }
    }

    //Render each moving platform held in the moving platforms vector in sequence.
    brush.amount_moving_platforms=0;
    for(int i=0;i<vector_moving_platforms.size();i++){
        if(brush.current_platform==i){
            vector_moving_platforms.at(i).render(true);
        }
        else{
            vector_moving_platforms.at(i).render(false);
        }
        brush.amount_moving_platforms++;
    }

    //Render each trigger held in the vector in sequence.
    brush.amount_triggers=0;
    for(int i=0;i<vector_triggers.size();i++){
        if(brush.current_trigger==i){
            vector_triggers[i].render(true);
        }
        else{
            vector_triggers[i].render(false);
        }
        brush.amount_triggers++;
    }

    //Render each door held in the vector in sequence.
    brush.amount_doors=0;
    brush.door_local_0=false;
    brush.door_local_1=false;
    brush.door_local_2=false;
    brush.door_local_3=false;
    brush.door_global_0=false;
    brush.door_global_1=false;
    brush.door_global_2=false;
    brush.door_global_3=false;
    brush.door_global_4=false;
    brush.door_global_5=false;
    brush.door_global_6=false;
    brush.door_global_7=false;
    brush.door_global_8=false;
    brush.door_global_9=false;
    brush.door_global_10=false;
    brush.door_global_11=false;
    for(int i=0;i<vector_doors.size();i++){
        if(brush.current_door==i){
            vector_doors[i].render(true);
        }
        else{
            vector_doors[i].render(false);
        }
        brush.amount_doors++;

        if(vector_doors[i].type==0 && vector_doors[i].number==0){
            brush.door_local_0=true;
        }
        else if(vector_doors[i].type==0 && vector_doors[i].number==1){
            brush.door_local_1=true;
        }
        else if(vector_doors[i].type==0 && vector_doors[i].number==2){
            brush.door_local_2=true;
        }
        else if(vector_doors[i].type==0 && vector_doors[i].number==3){
            brush.door_local_3=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==0){
            brush.door_global_0=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==1){
            brush.door_global_1=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==2){
            brush.door_global_2=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==3){
            brush.door_global_3=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==4){
            brush.door_global_4=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==5){
            brush.door_global_5=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==6){
            brush.door_global_6=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==7){
            brush.door_global_7=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==8){
            brush.door_global_8=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==9){
            brush.door_global_9=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==10){
            brush.door_global_10=true;
        }
        else if(vector_doors[i].type==1 && vector_doors[i].number==11){
            brush.door_global_11=true;
        }
    }

    //Render each trap held in the vector in sequence.
    brush.amount_trap_fire=0;
    for(int i=0;i<vector_traps.size();i++){
        if(brush.current_trap==i){
            vector_traps[i].render(true);
        }
        else{
            vector_traps[i].render(false);
        }

        if(vector_traps[i].type==1){
            brush.amount_trap_fire++;
        }
    }

    //Render each sign held in the vector in sequence.
    for(int i=0;i<vector_signs.size();i++){
        if(brush.current_sign==i){
            vector_signs[i].render(true);
        }
        else{
            vector_signs[i].render(false);
        }
    }

    for(int i=0;i<vector_springs.size();i++){
        if(brush.current_spring==i){
            vector_springs[i].render(true);
        }
        else{
            vector_springs[i].render(false);
        }
    }

    for(int i=0;i<vector_boosters.size();i++){
        if(brush.current_booster==i){
            vector_boosters[i].render(true);
        }
        else{
            vector_boosters[i].render(false);
        }
    }

    //Render each npc held in the npcs vector in sequence.
    for(int i=0;i<44;i++){
        brush.amount_npcs[i]=0;
    }
    for(int i=0;i<vector_npcs.size();i++){
        vector_npcs.at(i).render();
        brush.amount_npcs[vector_npcs[i].type]++;
    }

    //Render each item held in the items vector in sequence.
    brush.amount_leaves=0;
    brush.amount_cheese=0;
    brush.amount_ammo=0;
    brush.amount_spawnpoint=0;
    brush.amount_checkpoint=0;
    brush.amount_endpoint=0;
    brush.amount_swimming_gear=0;
    brush.amount_key_red=0;
    brush.amount_key_blue=0;
    brush.amount_key_green=0;
    brush.amount_key_yellow=0;
    brush.amount_key_orange=0;
    brush.amount_key_purple=0;
    brush.amount_towel=0;
    brush.amount_key_gray=0;
    brush.amount_key_brown=0;
    brush.amount_key_black=0;
    brush.amount_key_pink=0;
    brush.amount_key_cyan=0;
    brush.amount_sink=0;
    brush.amount_j_balloon=0;
    brush.amount_ammo_barrel=0;
    brush.amount_candy=0;
    brush.amount_suit_deadly_water=0;
    brush.amount_suit_sharp=0;
    brush.amount_suit_banana=0;
    brush.amount_shot_homing=0;
    brush.amount_translator=0;
    brush.amount_j_wing=0;
    for(int i=0;i<vector_items.size();i++){
        if(brush.item_selected==i){
            vector_items.at(i).render(true);
        }
        else{
            vector_items.at(i).render(false);
        }

        if(vector_items.at(i).type==ITEM_LEAF){
            brush.amount_leaves++;
        }
        else if(vector_items.at(i).type==ITEM_CHEESE){
            brush.amount_cheese++;
        }
        else if(vector_items.at(i).type==ITEM_AMMO){
            brush.amount_ammo++;
        }
        else if(vector_items.at(i).type==ITEM_SPAWNPOINT){
            brush.amount_spawnpoint++;
        }
        else if(vector_items.at(i).type==ITEM_CHECKPOINT){
            brush.amount_checkpoint++;
        }
        else if(vector_items.at(i).type==ITEM_ENDPOINT){
            brush.amount_endpoint++;
        }
        else if(vector_items.at(i).type==ITEM_SWIMMING_GEAR){
            brush.amount_swimming_gear++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_RED){
            brush.amount_key_red++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_BLUE){
            brush.amount_key_blue++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_GREEN){
            brush.amount_key_green++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_YELLOW){
            brush.amount_key_yellow++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_ORANGE){
            brush.amount_key_orange++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_PURPLE){
            brush.amount_key_purple++;
        }
        else if(vector_items.at(i).type==ITEM_TOWEL){
            brush.amount_towel++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_GRAY){
            brush.amount_key_gray++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_BROWN){
            brush.amount_key_brown++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_BLACK){
            brush.amount_key_black++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_PINK){
            brush.amount_key_pink++;
        }
        else if(vector_items.at(i).type==ITEM_KEY_CYAN){
            brush.amount_key_cyan++;
        }
        else if(vector_items.at(i).type==ITEM_SINK){
            brush.amount_sink++;
        }
        else if(vector_items.at(i).type==ITEM_J_BALLOON){
            brush.amount_j_balloon++;
        }
        else if(vector_items.at(i).type==ITEM_AMMO_BARREL){
            brush.amount_ammo_barrel++;
        }
        else if(vector_items.at(i).type==ITEM_CANDY){
            brush.amount_candy++;
        }
        else if(vector_items.at(i).type==ITEM_SUIT_DEADLY_WATER){
            brush.amount_suit_deadly_water++;
        }
        else if(vector_items.at(i).type==ITEM_SUIT_SHARP){
            brush.amount_suit_sharp++;
        }
        else if(vector_items.at(i).type==ITEM_SUIT_BANANA){
            brush.amount_suit_banana++;
        }
        else if(vector_items.at(i).type==ITEM_SHOT_HOMING){
            brush.amount_shot_homing++;
        }
        else if(vector_items.at(i).type==ITEM_TRANSLATOR){
            brush.amount_translator++;
        }
        else if(vector_items.at(i).type==ITEM_J_WING){
            brush.amount_j_wing++;
        }
    }

    //Render the brush.
    brush.render();

    if(brush.layer_current!=1 && brush.layer_current!=4 && brush.layer_current!=5 && brush.layer_current!=6 && brush.layer_current!=7){
        render_rectangle(0,0,400,200,.65,.36,.56,.95);
    }
    else if(brush.layer_current==1){
        render_rectangle(0,0,400,650,.65,.36,.56,.95);
    }
    else if(brush.layer_current==4 && brush.current_platform==-1){
        render_rectangle(0,0,400,100,.65,.36,.56,.95);
    }
    else if(brush.layer_current==4 && brush.current_platform!=-1){
        render_rectangle(0,0,400,400,.65,.36,.56,.95);
    }
    else if(brush.layer_current==5 && brush.current_trigger==-1){
        render_rectangle(0,0,400,100,.65,.36,.56,.95);
    }
    else if(brush.layer_current==5 && brush.current_trigger!=-1){
        render_rectangle(0,0,400,400,.65,.36,.56,.95);
    }
    else if(brush.layer_current==6 && brush.current_door==-1){
        render_rectangle(0,0,400,600,.65,.36,.56,.95);
    }
    else if(brush.layer_current==6 && brush.current_door!=-1){
        render_rectangle(0,0,400,600,.65,.36,.56,.95);
    }
    else if(brush.layer_current==7 && brush.current_trap==-1){
        render_rectangle(0,0,400,100,.65,.36,.56,.95);
    }
    else if(brush.layer_current==7 && brush.current_trap!=-1){
        render_rectangle(0,0,400,400,.65,.36,.56,.95);
    }

    if(brush.layer_current==0){
        //The current tile location for the brush.
        int brush_current_x=(int)((int)brush.x/32);
        int brush_current_y=(int)((int)brush.y/32);

        //Check all tiles in a square around the brush.
        int check_x_start=brush_current_x-4;
        int check_x_end=brush_current_x+4;
        int check_y_start=brush_current_y-4;
        int check_y_end=brush_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){

                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/32)-1 && int_y>=0 && int_y<=(level.level_y/32)-1){
                    if(collision_check(brush.x,brush.y,brush.w,brush.h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_W,TILE_H)){
                        ss.clear();ss.str("");ss<<"Soli<d>ity: ";ss<<level.tile_array[int_x][int_y].solidity;ss<<"\xA";msg=ss.str();
                        ss.clear();ss.str("");ss<<"S<p>rite: ";ss<<level.tile_array[int_x][int_y].sprite;ss<<"\xA";msg+=ss.str();
                        ss.clear();ss.str("");ss<<"<G> Special: ";ss<<level.tile_array[int_x][int_y].special;ss<<"\xA";msg+=ss.str();
                        ss.clear();ss.str("");ss<<"<F>oreground: ";ss<<level.tile_array[int_x][int_y].foreground;ss<<"\xA";msg+=ss.str();
                        ss.clear();ss.str("");ss<<"Sloping Ground <A>ngle: ";ss<<level.tile_array[int_x][int_y].angle;ss<<"\xA";msg+=ss.str();
                        ss.clear();ss.str("");ss<<"Toggle <H>eight Mask";ss<<"\xA";msg+=ss.str();
                        font.show(5,5,msg,COLOR_WHITE);
                    }
                }
            }
        }

        string hazards="Hazard Tiles: ";
        if(brush.hazard_tiles==-1){
            hazards+="Not Checked";
        }
        else{
            ss.clear();ss.str("");ss<<brush.hazard_tiles;hazards+=ss.str();
        }
        font.show(5,185,hazards,COLOR_WHITE);
    }
    else if(brush.layer_current==2){
        //The current tile location for the brush.
        int brush_current_x=(int)((int)brush.x/32);
        int brush_current_y=(int)((int)brush.y/32);

        //Check all tiles in a square around the brush.
        int check_x_start=brush_current_x-4;
        int check_x_end=brush_current_x+4;
        int check_y_start=brush_current_y-4;
        int check_y_end=brush_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){

                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/32)-1 && int_y>=0 && int_y<=(level.level_y/32)-1){
                    if(collision_check(brush.x,brush.y,brush.w,brush.h,level.tile_background_array[int_x][int_y].x,level.tile_background_array[int_x][int_y].y,TILE_W,TILE_H)){
                        ss.clear();ss.str("");ss<<"Sprite: ";ss<<level.tile_background_array[int_x][int_y].sprite;msg=ss.str();
                        font.show(5,5,msg,COLOR_WHITE);
                    }
                }
            }
        }
    }
    else if(brush.layer_current==1){
        msg="Current Item: ";

        if(brush.item_current==0 && brush.WORLD_MAP){
            msg+="Level 0";
        }
        else if(brush.item_current<0){
            ss.clear();ss.str("");ss<<"Level ";ss<<fabs(brush.item_current);msg+=ss.str();
        }
        else if(brush.item_current==0 && !brush.WORLD_MAP){
            msg+="None";
        }
        else if(brush.item_current==ITEM_LEAF){
            msg+="Leaf";
        }
        else if(brush.item_current==ITEM_CHEESE){
            msg+="Cheese";
        }
        else if(brush.item_current==ITEM_AMMO){
            msg+="Ammo Box";
        }
        else if(brush.item_current==ITEM_SPAWNPOINT){
            msg+="Spawn Point";
        }
        else if(brush.item_current==ITEM_CHECKPOINT){
            msg+="Check Point";
        }
        else if(brush.item_current==ITEM_ENDPOINT){
            msg+="End Point";
        }
        else if(brush.item_current==ITEM_SWIMMING_GEAR){
            msg+="Swimming Gear";
        }
        else if(brush.item_current==ITEM_KEY_RED){
            msg+="Red Key";
        }
        else if(brush.item_current==ITEM_KEY_BLUE){
            msg+="Blue Key";
        }
        else if(brush.item_current==ITEM_KEY_GREEN){
            msg+="Green Key";
        }
        else if(brush.item_current==ITEM_KEY_YELLOW){
            msg+="Yellow Key";
        }
        else if(brush.item_current==ITEM_KEY_ORANGE){
            msg+="Orange Key";
        }
        else if(brush.item_current==ITEM_KEY_PURPLE){
            msg+="Purple Key";
        }
        else if(brush.item_current==ITEM_TOWEL){
            msg+="Towel";
        }
        else if(brush.item_current==ITEM_KEY_GRAY){
            msg+="Gray Key";
        }
        else if(brush.item_current==ITEM_KEY_BROWN){
            msg+="Brown Key";
        }
        else if(brush.item_current==ITEM_KEY_BLACK){
            msg+="Black Key";
        }
        else if(brush.item_current==ITEM_KEY_PINK){
            msg+="Pink Key";
        }
        else if(brush.item_current==ITEM_KEY_CYAN){
            msg+="Cyan Key";
        }
        else if(brush.item_current==ITEM_SINK){
            msg+="Sink";
        }
        else if(brush.item_current==ITEM_J_BALLOON){
            msg+="J-Balloon";
        }
        else if(brush.item_current==ITEM_AMMO_BARREL){
            msg+="Ammo Barrel";
        }
        else if(brush.item_current==ITEM_CANDY){
            msg+="Candy";
        }
        else if(brush.item_current==ITEM_SUIT_DEADLY_WATER){
            msg+="Deadly Water Suit";
        }
        else if(brush.item_current==ITEM_SUIT_SHARP){
            msg+="Sharp Suit";
        }
        else if(brush.item_current==ITEM_SUIT_BANANA){
            msg+="Banana Suit";
        }
        else if(brush.item_current==ITEM_SHOT_HOMING){
            msg+="Homing Shot";
        }
        else if(brush.item_current==ITEM_TRANSLATOR){
            msg+="Translator";
        }
        else if(brush.item_current==ITEM_J_WING){
            msg+="J-Wing";
        }

        font.show(5,5,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Leaves: ";ss<<brush.amount_leaves;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Cheese: ";ss<<brush.amount_cheese;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Ammo Boxes: ";ss<<brush.amount_ammo;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Spawn Points: ";ss<<brush.amount_spawnpoint;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Check Points: ";ss<<brush.amount_checkpoint;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"End Points: ";ss<<brush.amount_endpoint;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Swimming Gear: ";ss<<brush.amount_swimming_gear;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Red Key: ";ss<<brush.amount_key_red;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Blue Key: ";ss<<brush.amount_key_blue;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Green Key: ";ss<<brush.amount_key_green;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Yellow Key: ";ss<<brush.amount_key_yellow;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Orange Key: ";ss<<brush.amount_key_orange;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Purple Key: ";ss<<brush.amount_key_purple;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Towel: ";ss<<brush.amount_towel;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Gray Key: ";ss<<brush.amount_key_gray;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Brown Key: ";ss<<brush.amount_key_brown;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Black Key: ";ss<<brush.amount_key_black;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Pink Key: ";ss<<brush.amount_key_pink;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Cyan Key: ";ss<<brush.amount_key_cyan;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Sink: ";ss<<brush.amount_sink;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"J-Balloon: ";ss<<brush.amount_j_balloon;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Ammo Barrel: ";ss<<brush.amount_ammo_barrel;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Candy: ";ss<<brush.amount_candy;ss<<"\xA";msg+=ss.str();
        /**ss.clear();ss.str("");ss<<"Deadly Water Suit: ";ss<<brush.amount_suit_deadly_water;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Sharp Suit: ";ss<<brush.amount_suit_sharp;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Banana Suit: ";ss<<brush.amount_suit_banana;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Homing Shot: ";ss<<brush.amount_shot_homing;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Translator: ";ss<<brush.amount_translator;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"J-Wing: ";ss<<brush.amount_j_wing;ss<<"\xA";msg+=ss.str();*/
        if(brush.item_selected!=-1){
            ss.clear();ss.str("");ss<<"\xA";ss<<"<G>oal_level_to_load: ";ss<<vector_items[brush.item_selected].goal_level_to_load;msg+=ss.str();
            ss.clear();ss.str("");ss<<"\xA";ss<<"goal_<S>ecret: ";ss<<vector_items[brush.item_selected].goal_secret;ss<<"\xA";msg+=ss.str();
        }

        font.show(5,30,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==3){
        msg="Current Npc: ";

        msg+=brush.return_npc_name(brush.npc_current);

        font.show(5,5,msg,COLOR_WHITE);

        msg="";
        for(int i=0;i<44;i++){
            if(brush.amount_npcs[i]>0){
                ss.clear();ss.str("");ss<<brush.return_npc_name(i);ss<<": ";ss<<brush.amount_npcs[i];ss<<"\xA";msg+=ss.str();
            }
        }

        font.show(5,30,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==4){
        ss.clear();ss.str("");ss<<"Moving Platforms: ";ss<<brush.amount_moving_platforms;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"<Enter> Select platform for";ss<<"\xA";ss<<"end point/waypoint editing";ss<<"\xA";ss<<"\xA";msg+=ss.str();

        //If there is currently a selected platform.
        if(brush.current_platform!=-1){
            ss.clear();ss.str("");ss<<"Move <S>peed: ";ss<<vector_moving_platforms[brush.current_platform].move_speed;ss<<"\xA";msg+=ss.str();
            if(vector_moving_platforms[brush.current_platform].active){
                ss.clear();ss.str("");ss<<"<A>ctive: Yes";ss<<"\xA";msg+=ss.str();
            }
            else if(!vector_moving_platforms[brush.current_platform].active){
                ss.clear();ss.str("");ss<<"<A>ctive: No";ss<<"\xA";msg+=ss.str();
            }
            if(vector_moving_platforms[brush.current_platform].round_trip){
                ss.clear();ss.str("");ss<<"<R>ound Trip: Yes";ss<<"\xA";msg+=ss.str();
            }
            else if(!vector_moving_platforms[brush.current_platform].round_trip){
                ss.clear();ss.str("");ss<<"<R>ound Trip: No";ss<<"\xA";msg+=ss.str();
            }
            if(!vector_moving_platforms[brush.current_platform].movement_type){
                ss.clear();ss.str("");ss<<"<M>ovement Type: Back and Forth";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_moving_platforms[brush.current_platform].movement_type){
                ss.clear();ss.str("");ss<<"<M>ovement Type: Circular";ss<<"\xA";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"<J>ump to End Point";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Place <E>nd Point";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Waypoints: ";ss<<vector_moving_platforms[brush.current_platform].waypoints.size();ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Place <W>aypoint";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<Q> Remove Waypoint";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<P>lace Start Point";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Identifier: ";ss<<vector_moving_platforms[brush.current_platform].identifier;ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==5){
        ss.clear();ss.str("");ss<<"Triggers: ";ss<<brush.amount_triggers;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"<Enter> Select trigger for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg+=ss.str();

        //If there is currently a selected trigger.
        if(brush.current_trigger!=-1){
            if(vector_triggers[brush.current_trigger].trigger_method){
                ss.clear();ss.str("");ss<<"<T>rigger Method: Touch";ss<<"\xA";msg+=ss.str();
            }
            else{
                ss.clear();ss.str("");ss<<"<T>rigger Method: Manual Activation";ss<<"\xA";msg+=ss.str();
            }
            if(vector_triggers[brush.current_trigger].user_type==0){
                ss.clear();ss.str("");ss<<"<U>ser Type: No NPCs";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_triggers[brush.current_trigger].user_type==1){
                ss.clear();ss.str("");ss<<"<U>ser Type: All";ss<<"\xA";msg+=ss.str();
            }
            if(vector_triggers[brush.current_trigger].repeating){
                ss.clear();ss.str("");ss<<"<R>epeating: On";ss<<"\xA";msg+=ss.str();
            }
            else{
                ss.clear();ss.str("");ss<<"<R>epeating: Off";ss<<"\xA";msg+=ss.str();
            }
            if(vector_triggers[brush.current_trigger].render_trigger==0){
                ss.clear();ss.str("");ss<<"R<e>nder: Off";ss<<"\xA";msg+=ss.str();
            }
            else{
                ss.clear();ss.str("");ss<<"R<e>nder: ";ss<<vector_triggers[brush.current_trigger].render_trigger;ss<<"\xA";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"Re<p>eat Time: ";ss<<vector_triggers[brush.current_trigger].repeat_time;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"</ *> Adjust width.";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<- +> Adjust height.";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<M>ove Trigger";ss<<"\xA";msg+=ss.str();
            ///ss.clear();ss.str("");ss<<"<J>ump to Target";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<Right Shift> Select Target";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<Forward Slash> Deselect Target";ss<<"\xA";msg+=ss.str();
            ///ss.clear();ss.str("");ss<<"Target Identifier: ";ss<<vector_triggers[brush.current_trigger].target_identifier;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Targets: ";ss<<vector_triggers[brush.current_trigger].targets.size();ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==6){
        ss.clear();ss.str("");ss<<"Doors: ";ss<<brush.amount_doors;ss<<"\xA";msg=ss.str();

        ss.clear();ss.str("");ss<<"Local 0: ";ss<<brush.door_local_0;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Local 1: ";ss<<brush.door_local_1;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Local 2: ";ss<<brush.door_local_2;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Local 3: ";ss<<brush.door_local_3;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 0: ";ss<<brush.door_global_0;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 1: ";ss<<brush.door_global_1;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 2: ";ss<<brush.door_global_2;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 3: ";ss<<brush.door_global_3;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 4: ";ss<<brush.door_global_4;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 5: ";ss<<brush.door_global_5;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 6: ";ss<<brush.door_global_6;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 7: ";ss<<brush.door_global_7;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 8: ";ss<<brush.door_global_8;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 9: ";ss<<brush.door_global_9;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 10: ";ss<<brush.door_global_10;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Global 11: ";ss<<brush.door_global_11;ss<<"\xA";msg+=ss.str();

        ss.clear();ss.str("");ss<<"<Enter> Select door for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg+=ss.str();

        //If there is currently a selected door.
        if(brush.current_door!=-1){
            if(vector_doors[brush.current_door].open){
                ss.clear();ss.str("");ss<<"<O>pen State: Open";ss<<"\xA";msg+=ss.str();
            }
            else{
                ss.clear();ss.str("");ss<<"<O>pen State: Closed";ss<<"\xA";msg+=ss.str();
            }
            if(vector_doors[brush.current_door].type==0){
                ss.clear();ss.str("");ss<<"<T>ype: Local Level Door";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_doors[brush.current_door].type==1){
                ss.clear();ss.str("");ss<<"<T>ype: Key Door";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_doors[brush.current_door].type==2){
                ss.clear();ss.str("");ss<<"<T>ype: Standard Door";ss<<"\xA";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"Door <N>umber: ";ss<<vector_doors[brush.current_door].number;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<M>ove Door";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Identifier: ";ss<<vector_doors[brush.current_door].identifier;ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==7){
        msg="Current Trap: ";

        if(brush.trap_current==0){
            msg+="None";
        }
        else if(brush.trap_current==1){
            msg+="Fire";
        }
        else if(brush.trap_current==2){
            msg+="Spikes (Moving)";
        }
        else if(brush.trap_current==3){
            msg+="Icicle";
        }
        else if(brush.trap_current==4){
            msg+="Branch";
        }
        else if(brush.trap_current==5){
            msg+="Electric";
        }
        else if(brush.trap_current==6){
            msg+="Falling Stone Slab";
        }
        else if(brush.trap_current==7){
            msg+="Buzzsaw";
        }
        else if(brush.trap_current==8){
            msg+="Alarm Toggler";
        }
        else if(brush.trap_current==9){
            msg+="Weather Machine";
        }
        else if(brush.trap_current==10){
            msg+="Spikes Up";
        }
        else if(brush.trap_current==11){
            msg+="Spikes Down";
        }
        else if(brush.trap_current==12){
            msg+="Spikes Right";
        }
        else if(brush.trap_current==13){
            msg+="Spikes Left";
        }
        else if(brush.trap_current==14){
            msg+="Boss - Castle";
        }
        else if(brush.trap_current==15){
            msg+="Boss - Pyramid";
        }
        else if(brush.trap_current==16){
            msg+="Seaweed";
        }
        else if(brush.trap_current==17){
            msg+="Open Shop";
        }
        else if(brush.trap_current==18){
            msg+="Open Upgrades";
        }

        msg+="\xA";

        ss.clear();ss.str("");ss<<"Fire: ";ss<<brush.amount_trap_fire;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"<Enter> Select trap for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg+=ss.str();

        //If there is currently a selected trap.
        if(brush.current_trap!=-1){
            if(vector_traps[brush.current_trap].active){
                ss.clear();ss.str("");ss<<"<A>ctive";ss<<"\xA";msg+=ss.str();
            }
            else{
                ss.clear();ss.str("");ss<<"In<a>ctive";ss<<"\xA";msg+=ss.str();
            }
            if(vector_traps[brush.current_trap].type==1){
                ss.clear();ss.str("");ss<<"<T>ype: Fire";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==2){
                ss.clear();ss.str("");ss<<"<T>ype: Spikes";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==3){
                ss.clear();ss.str("");ss<<"<T>ype: Icicle";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==4){
                ss.clear();ss.str("");ss<<"<T>ype: Branch";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==5){
                ss.clear();ss.str("");ss<<"<T>ype: Electric";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==6){
                ss.clear();ss.str("");ss<<"<T>ype: Falling stone slab";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==7){
                ss.clear();ss.str("");ss<<"<T>ype: Buzzsaw";ss<<"\xA";msg+=ss.str();
            }
            else if(vector_traps[brush.current_trap].type==8){
                ss.clear();ss.str("");ss<<"<T>ype: Alarm Toggler";ss<<"\xA";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"<M>ove Trap";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Identifier: ";ss<<vector_traps[brush.current_trap].identifier;ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==8){
        ss.clear();ss.str("");ss<<"<Enter> Select sign for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg=ss.str();

        //If there is currently a selected sign.
        if(brush.current_sign!=-1){
            ss.clear();ss.str("");ss<<"<M>ove Sign";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"M<e>ssage: ";ss<<vector_signs[brush.current_sign].message;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<T>ype: ";ss<<vector_signs[brush.current_sign].type;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"<F>ont Type: ";ss<<vector_signs[brush.current_sign].font_type;ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==9){
        ss.clear();ss.str("");ss<<"<Enter> Select spring for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg=ss.str();

        //If there is currently a selected spring.
        if(brush.current_spring!=-1){
            ss.clear();ss.str("");ss<<"<D>irection: ";msg+=ss.str();
            if(vector_springs[brush.current_spring].direction==LEFT){
                ss.clear();ss.str("");ss<<"Left";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==UP){
                ss.clear();ss.str("");ss<<"Up";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==RIGHT){
                ss.clear();ss.str("");ss<<"Right";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==DOWN){
                ss.clear();ss.str("");ss<<"Down";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==LEFT_UP){
                ss.clear();ss.str("");ss<<"Left Up";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==LEFT_DOWN){
                ss.clear();ss.str("");ss<<"Left Down";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==RIGHT_UP){
                ss.clear();ss.str("");ss<<"Right Up";msg+=ss.str();
            }
            else if(vector_springs[brush.current_spring].direction==RIGHT_DOWN){
                ss.clear();ss.str("");ss<<"Right Down";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"\xA";ss<<"<S>peed: ";ss<<vector_springs[brush.current_spring].speed;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"\xA";ss<<"<M>ove Spring";ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==10){
        ss.clear();ss.str("");ss<<"<Enter> Select booster for";ss<<"\xA";ss<<"editing";ss<<"\xA";ss<<"\xA";msg=ss.str();

        //If there is currently a selected booster.
        if(brush.current_booster!=-1){
            ss.clear();ss.str("");ss<<"<D>irection: ";msg+=ss.str();
            if(vector_boosters[brush.current_booster].direction==LEFT){
                ss.clear();ss.str("");ss<<"Left";msg+=ss.str();
            }
            else if(vector_boosters[brush.current_booster].direction==RIGHT){
                ss.clear();ss.str("");ss<<"Right";msg+=ss.str();
            }
            ss.clear();ss.str("");ss<<"\xA";ss<<"<S>peed: ";ss<<vector_boosters[brush.current_booster].speed;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"\xA";ss<<"<M>ove Booster";ss<<"\xA";msg+=ss.str();
        }

        font.show(5,5,msg,COLOR_WHITE);
    }

    render_rectangle(640,0,400,210,.65,.36,.56,.95);

    font.show(650,10,"Level Size",COLOR_WHITE);
    if(brush.WORLD_MAP){
        font.show(850,10,"World Map Mode");
    }
    ss.clear();ss.str("");ss<<"Tiles: ";ss<<level.level_x/32;ss<<" x ";ss<<level.level_y/32;msg=ss.str();
    font.show(670,35,msg,COLOR_WHITE);
    ss.clear();ss.str("");ss<<"Pixels: ";ss<<level.level_x;ss<<" x ";ss<<level.level_y;msg=ss.str();
    font.show(670,60,msg,COLOR_WHITE);

    if(brush.layer_current==0){
        ss.clear();ss.str("");ss<<"Current Tile: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Terrain tiles";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==2){
        ss.clear();ss.str("");ss<<"Current Tile: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Background tiles";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==1){
        ss.clear();ss.str("");ss<<"Current Item: (";ss<<brush.x/ITEM_SIZE;ss<<" , ";ss<<brush.y/ITEM_SIZE;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Items";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==3){
        ss.clear();ss.str("");ss<<"Current Npc: (";ss<<brush.x/NPC_SIZE;ss<<" , ";ss<<brush.y/NPC_SIZE;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Npcs";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==4){
        ss.clear();ss.str("");ss<<"Current Moving Platform: (";ss<<brush.x/16;ss<<" , ";ss<<brush.y/16;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Moving Platforms";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==5){
        ss.clear();ss.str("");ss<<"Current Trigger: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Triggers";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==6){
        ss.clear();ss.str("");ss<<"Current Door: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Doors";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==7){
        ss.clear();ss.str("");ss<<"Current Trap: (";ss<<brush.x/16;ss<<" , ";ss<<brush.y/16;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Traps";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==8){
        ss.clear();ss.str("");ss<<"Current Sign: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Signs";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==9){
        ss.clear();ss.str("");ss<<"Current Spring: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Springs";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }
    else if(brush.layer_current==10){
        ss.clear();ss.str("");ss<<"Current Booster: (";ss<<brush.x/32;ss<<" , ";ss<<brush.y/32;ss<<")";msg=ss.str();
        font.show(650,85,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Current Layer: Boosters";msg=ss.str();
        font.show(650,110,msg,COLOR_WHITE);
    }

    //Display the framerate and milliseconds per frame.
    ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;ss<<"\xA";msg=ss.str();
    ss.clear();ss.str("");ss<<"MS Per Frame: ";ss<<ms_per_frame;ss<<"\xA";msg+=ss.str();
    if(brush.speed_mode){
        ss.clear();ss.str("");ss<<"Speed Mode";ss<<"\xA";msg+=ss.str();
    }
    font.show(650,135,msg,COLOR_WHITE);

    //Display help.
    if(brush.option_display_help==true){
        render_rectangle(brush.window_big_x,brush.window_big_y,1440,900,.65,.36,.56,.95);
        font.show(brush.window_big_x+545,brush.window_big_y+60,"F1 = Generate a new level.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+85,"F2 = Load Level",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+110,"F3 = Save Level",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+135,"F4 = Randomly generate a new level.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+160,"F5 = Save a screenshot.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+185,"F10 = Exit the editor.",COLOR_WHITE);

        font.show(brush.window_big_x+545,brush.window_big_y+210,"Left Bracket = Toggle display of overlays.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+235,"Right Bracket = Toggle display of grid.",COLOR_WHITE);

        font.show(brush.window_big_x+545,brush.window_big_y+260,"D = Toggle solidity of the tile.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+285,"S = Toggle secrecy of the tile.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+310,"P = Select the sprite for the tile.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+335,"T = Generate clutter.",COLOR_WHITE);

        font.show(brush.window_big_x+545,brush.window_big_y+360,"C = Copy tile.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+385,"V = Paste tile.",COLOR_WHITE);
        font.show(brush.window_big_x+545,brush.window_big_y+410,"E = Erase tile.",COLOR_WHITE);

        font.show(brush.window_big_x+545,brush.window_big_y+435,"H = Toggle this help window.",COLOR_WHITE);
    }

    //Setting move_speed.
    if(brush.edit_input==1){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new move_speed number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting spring speed.
    if(brush.edit_input==22){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new speed number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting booster speed.
    if(brush.edit_input==24){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new speed number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting goal_level_to_load.
    if(brush.edit_input==19){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new goal_level_to_load number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting repeat_time.
    else if(brush.edit_input==13){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new repeat time number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting new level dimensions.
    else if(brush.edit_input==3){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        if(brush.edit_new_x==0){
            font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new level's x size and press enter when done:",COLOR_WHITE);
        }
        else{
            font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new level's y size and press enter when done:",COLOR_WHITE);
        }
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting level resize dimensions.
    else if(brush.edit_input==21){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        if(brush.edit_new_x==0){
            font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the level's x size and press enter when done:",COLOR_WHITE);
        }
        else{
            font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the level's y size and press enter when done:",COLOR_WHITE);
        }
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting sloping ground angle.
    else if(brush.edit_input==11){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the new slope number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting height mask.
    else if(brush.edit_input==15){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the identifier for the new height mask and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting sign message.
    else if(brush.edit_input==17){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the message for the sign:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
    }

    //Setting sprite.
    else if(brush.edit_input==4){
        render_rectangle((brush.camera.w-1440)/2,(brush.camera.h-900)/2,1440,900,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+25,(brush.camera.h-480)/2-125,"Type the sprite number and press enter when done:",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+275,(brush.camera.h-480)/2-100,brush.edit_string,COLOR_WHITE);
        font.show((brush.camera.w-640)/2+75,(brush.camera.h-480)/2-50,"OR, click on the sprite you want to set:",COLOR_WHITE);
        //Render all of the tile sprites in a grid.
        if(!brush.WORLD_MAP && brush.layer_current==0){
            for(int i=0;i<TILE_SPRITES;i++){
                short row=set_tile_row(i);
                short column=set_tile_column(i);

                ss.clear();ss.str("");ss<<i;msg=ss.str();
                font.show((34*column),260+(row*63),msg,COLOR_WHITE);
                if(i!=0){
                    if(i>2){
                        render_sprite((34*column),285+(row*63),608,288,tiles,&brush.sprites_tiles[i]);
                    }
                    else{
                        render_sprite((34*column),285+(row*63),512,32,water_tiles,&brush.sprites_tiles[i]);
                    }
                }
                render_texture((34*column),285+(row*63),32,32,grid);
            }
        }
        else if(!brush.WORLD_MAP && brush.layer_current==2){
            for(int i=0;i<BACKGROUND_TILES;i++){
                short row=set_tile_row(i);
                short column=set_tile_column(i);

                ss.clear();ss.str("");ss<<i;msg=ss.str();
                font.show((34*column),260+(row*63),msg,COLOR_WHITE);
                if(i!=0){
                    render_sprite((34*column),285+(row*63),256,288,tiles_background,&brush.sprites_tiles_background[i]);
                }
                render_texture((34*column),285+(row*63),32,32,grid);
            }
        }
        else if(brush.WORLD_MAP){
            for(int i=0;i<WORLDMAP_TILES;i++){
                short row=set_tile_row(i);
                short column=set_tile_column(i);

                ss.clear();ss.str("");ss<<i;msg=ss.str();
                font.show((34*column),260+(row*63),msg,COLOR_WHITE);
                if(i!=0){
                    if(i>2){
                        render_sprite((34*column),285+(row*63),608,288,worldmap_tiles,&brush.worldmap_sprites_tiles[i]);
                    }
                    else{
                        render_sprite((34*column),285+(row*63),512,32,water_tiles,&brush.worldmap_sprites_tiles[i]);
                    }
                }
                render_texture((34*column),285+(row*63),32,32,grid);
            }
        }
    }

    //Display the quit confirmation dialog box.
    else if(brush.edit_input==6){
        render_rectangle((brush.camera.w-640)/2,(brush.camera.h-480)/2,640,480,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+250,(brush.camera.h-480)/2+200,"Really exit?",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+200,(brush.camera.h-480)/2+225,"Don't forget to SAVE!",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+240,(brush.camera.h-480)/2+250,"(Y)es or (N)o",COLOR_WHITE);
    }

    //Display the new confirmation dialog box.
    else if(brush.edit_input==7){
        render_rectangle((brush.camera.w-640)/2,(brush.camera.h-480)/2,640,480,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+160,(brush.camera.h-480)/2+200,"Really generate a new level?",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+240,(brush.camera.h-480)/2+225,"(Y)es or (N)o",COLOR_WHITE);
    }

    //Display the load confirmation dialog box.
    else if(brush.edit_input==8){
        render_rectangle((brush.camera.w-640)/2,(brush.camera.h-480)/2,640,480,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+200,(brush.camera.h-480)/2+200,"Really resize the level?",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+240,(brush.camera.h-480)/2+225,"(Y)es or (N)o",COLOR_WHITE);
    }

    //Display the save confirmation dialog box.
    else if(brush.edit_input==9){
        render_rectangle((brush.camera.w-640)/2,(brush.camera.h-480)/2,640,480,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+200,(brush.camera.h-480)/2+200,"Really save the level?",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+240,(brush.camera.h-480)/2+225,"(Y)es or (N)o",COLOR_WHITE);
    }

    //Display the new random confirmation dialog box.
    else if(brush.edit_input==10){
        render_rectangle((brush.camera.w-640)/2,(brush.camera.h-480)/2,640,480,.65,.36,.56,.95);
        font.show((brush.camera.w-640)/2+125,(brush.camera.h-480)/2+200,"Really generate a new random level?",COLOR_WHITE);
        font.show((brush.camera.w-640)/2+240,(brush.camera.h-480)/2+225,"(Y)es or (N)o",COLOR_WHITE);
    }

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
