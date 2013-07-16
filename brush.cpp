/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "brush.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "quit.h"
#include "random_number_generator.h"
#include "height_masks.h"

#include <fstream>

using namespace std;

Brush::Brush(){
    //Start the keystates variable as NULL.
    keystates=NULL;

    //Constants:
    LEFT=1;
    UP=2;
    RIGHT=3;
    DOWN=4;
    LEFT_UP=5;
    RIGHT_UP=6;
    RIGHT_DOWN=7;
    LEFT_DOWN=8;

    //Variables:

    //These first few variables handle input:

    //Start out not moving.
    move_state=0;

    //That's it for the input variables. These handle actual game physics:

    //Set the camera's initial location and its dimensions.
    camera.x=0;
    camera.y=0;
    camera.w=1280;
    camera.h=800;

    x=0;
    y=0;

    //Dimensions are predetermined.
    w=32;
    h=32;

    //Setup the height masks.

    //Load the number of height masks.
    ifstream load;
    string file_to_load="data/height_masks/number_of_masks.txt";
    load.open(file_to_load.c_str(),ifstream::in);

    if(load!=NULL){
        load>>number_of_height_masks;

        load.close();
        load.clear();
    }
    else{
        fprintf(stderr,"Failed to load number of height masks.\n");
    }

    height_masks.resize(number_of_height_masks,vector<short>(TILE_W));

    //Temporary vector used to load height mask data.
    vector<short> height_mask_loading;

    for(int n=0,height_mask_number=0;n<number_of_height_masks;n++,height_mask_number++){
        height_mask_loading.clear();
        string temp="";
        ss.clear();ss.str("");ss<<height_mask_number;temp=ss.str();
        height_mask_loading=load_height_mask(temp.c_str());
        for(short i=0;i<TILE_W;i++){
            height_masks[height_mask_number][i]=height_mask_loading[i];
        }
    }

    //Terrain tiles.
    sprites_box[0].x=0;
    sprites_box[0].y=0;
    sprites_box[0].w=32;
    sprites_box[0].h=32;

    //Items.
    sprites_box[1].x=32;
    sprites_box[1].y=0;
    sprites_box[1].w=16;
    sprites_box[1].h=16;

    //Background tiles.
    sprites_box[2].x=0;
    sprites_box[2].y=0;
    sprites_box[2].w=32;
    sprites_box[2].h=32;

    //Npcs.
    sprites_box[3].x=32;
    sprites_box[3].y=0;
    sprites_box[3].w=16;
    sprites_box[3].h=16;

    //Moving platforms.
    sprites_box[4].x=32;
    sprites_box[4].y=0;
    sprites_box[4].w=16;
    sprites_box[4].h=16;

    //Triggers.
    sprites_box[5].x=0;
    sprites_box[5].y=0;
    sprites_box[5].w=32;
    sprites_box[5].h=32;

    //Doors.
    sprites_box[6].x=0;
    sprites_box[6].y=0;
    sprites_box[6].w=32;
    sprites_box[6].h=32;

    //Traps.
    sprites_box[7].x=32;
    sprites_box[7].y=0;
    sprites_box[7].w=16;
    sprites_box[7].h=16;

    //Signs.
    sprites_box[8].x=0;
    sprites_box[8].y=0;
    sprites_box[8].w=32;
    sprites_box[8].h=32;

    //Springs.
    sprites_box[9].x=0;
    sprites_box[9].y=0;
    sprites_box[9].w=32;
    sprites_box[9].h=32;

    //Boosters.
    sprites_box[10].x=0;
    sprites_box[10].y=0;
    sprites_box[10].w=32;
    sprites_box[10].h=32;

    //Background tile sprite sheet.
    for(int column=0;column<8;column++){
        for(int i=1+9*column,n=0;i<10+9*column;i++,n++){
            sprites_tiles_background[i].x=TILE_W*column;
            sprites_tiles_background[i].y=n*TILE_H;
            sprites_tiles_background[i].w=TILE_W;
            sprites_tiles_background[i].h=TILE_H;
        }
    }

    for(short i=0;i<10;i++){
        sprites_overlays[i].x=0;
        sprites_overlays[i].y=i*32;
        sprites_overlays[i].w=w;
        sprites_overlays[i].h=h;
    }

    //Terrain tiles:

    //Water.
    sprites_tiles[1].x=TILE_W;
    sprites_tiles[1].y=0;
    sprites_tiles[1].w=TILE_W;
    sprites_tiles[1].h=TILE_H;

    sprites_tiles[2].x=0;
    sprites_tiles[2].y=0;
    sprites_tiles[2].w=TILE_W;
    sprites_tiles[2].h=TILE_H;

    //Ground.
    for(short i=3,n=0;i<=5;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=6,n=0;i<=8;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=9,n=0;i<=11;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*2;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=12,n=0;i<=14;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*3;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Angles.
    for(short i=15,n=0;i<=16;i++,n++){
        sprites_tiles[i].x=3*TILE_W+n*TILE_W;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=17,n=0;i<=18;i++,n++){
        sprites_tiles[i].x=3*TILE_W+n*TILE_W;
        sprites_tiles[i].y=TILE_H;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Climbable.
    for(short i=19,n=0;i<=22;i++,n++){
        sprites_tiles[i].x=3*TILE_W+n*TILE_W;
        sprites_tiles[i].y=TILE_H*2;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Other.
    for(short i=23,n=0;i<=29;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*4;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=30,n=0;i<=37;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*5;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=38,n=0;i<=45;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*6;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=46,n=0;i<=53;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*7;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=54,n=0;i<=61;i++,n++){
        sprites_tiles[i].x=n*TILE_W;
        sprites_tiles[i].y=TILE_H*8;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Clutter tiles.
    for(short i=62,n=0;i<=66;i++,n++){
        sprites_tiles[i].x=TILE_W*7;
        sprites_tiles[i].y=TILE_H*n;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Other ground edges.
    for(short i=67,n=0;i<=68;i++,n++){
        sprites_tiles[i].x=TILE_W*5+TILE_W*n;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=69,n=0;i<=70;i++,n++){
        sprites_tiles[i].x=TILE_W*5+TILE_W*n;
        sprites_tiles[i].y=TILE_H;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=71,n=0;i<=73;i++,n++){
        sprites_tiles[i].x=TILE_W*3+TILE_W*n;
        sprites_tiles[i].y=TILE_H*3;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //Other
    for(short i=74,n=0;i<=84;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=85,n=0;i<=95;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=96,n=0;i<=106;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*2;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=107,n=0;i<=117;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*3;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=118,n=0;i<=128;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*4;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=129,n=0;i<=139;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*5;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=140,n=0;i<=150;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*6;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=151,n=0;i<=161;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*7;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
    for(short i=162,n=0;i<=172;i++,n++){
        sprites_tiles[i].x=TILE_W*8+TILE_W*n;
        sprites_tiles[i].y=TILE_H*8;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }

    //World map tiles:

    //Water.
    worldmap_sprites_tiles[1].x=TILE_W;
    worldmap_sprites_tiles[1].y=0;
    worldmap_sprites_tiles[1].w=TILE_W;
    worldmap_sprites_tiles[1].h=TILE_H;

    worldmap_sprites_tiles[2].x=0;
    worldmap_sprites_tiles[2].y=0;
    worldmap_sprites_tiles[2].w=TILE_W;
    worldmap_sprites_tiles[2].h=TILE_H;

    for(int i=3;i<=11;i++){
        worldmap_sprites_tiles[i].x=0;
        worldmap_sprites_tiles[i].y=(i-3)*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=12,n=0;i<=20;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=21,n=0;i<=29;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*2;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=30,n=0;i<=38;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*3;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=39,n=0;i<=47;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*4;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=48,n=0;i<=56;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*5;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=57,n=0;i<=65;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*6;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=66,n=0;i<=74;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*7;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=75,n=0;i<=83;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*8;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=84,n=0;i<=92;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*9;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=93,n=0;i<=101;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*10;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=102,n=0;i<=110;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*11;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=111,n=0;i<=119;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*12;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=120,n=0;i<=128;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*13;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=129,n=0;i<=137;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*14;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=138,n=0;i<=146;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*15;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=147,n=0;i<=155;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*16;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=156,n=0;i<=164;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*17;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    for(int i=165,n=0;i<=173;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_W*18;
        worldmap_sprites_tiles[i].y=n*TILE_H;
        worldmap_sprites_tiles[i].w=TILE_W;
        worldmap_sprites_tiles[i].h=TILE_H;
    }

    desert_addition=false;

    //Editing variables:

    layer_tiles=true;
    layer_items=true;
    layer_tiles_background=true;
    layer_npcs=true;
    layer_moving_platforms=true;
    layer_triggers=true;
    layer_doors=true;
    layer_traps=true;
    layer_signs=true;
    layer_springs=true;
    layer_boosters=true;

    layer_current=0;

    item_current=0;
    item_selected=-1;

    npc_current=0;
    trap_current=0;

    edit_input=0;

    new_level_is_random=false;

    ///I don't think these are used.
    edit_solidity=0;

    edit_sprite=0;

    edit_move_speed=0.0;

    edit_goal_level_to_load=0;

    edit_string="";

    //-1 means that no platform is selected.
    current_platform=-1;

    //-1 means that no trigger is selected.
    current_trigger=-1;

    current_door=-1;

    current_trap=-1;

    current_sign=-1;

    current_spring=-1;

    current_booster=-1;

    option_display_help=false;
    option_display_overlays=true;
    option_display_grid=true;

    MAX_BRUSH_SIZE=30;

    reset_copy();

    hazard_tiles=-1;

    amount_leaves=0;
    amount_cheese=0;
    amount_ammo=0;
    amount_spawnpoint=0;
    amount_checkpoint=0;
    amount_endpoint=0;
    amount_swimming_gear=0;
    amount_key_red=0;
    amount_key_blue=0;
    amount_key_green=0;
    amount_key_yellow=0;
    amount_key_orange=0;
    amount_key_purple=0;
    amount_towel=0;
    amount_key_gray=0;
    amount_key_brown=0;
    amount_key_black=0;
    amount_key_pink=0;
    amount_key_cyan=0;
    amount_sink=0;
    amount_j_balloon=0;
    amount_ammo_barrel=0;
    amount_candy=0;
    amount_suit_deadly_water=0;
    amount_suit_sharp=0;
    amount_suit_banana=0;
    amount_shot_homing=0;
    amount_translator=0;
    amount_j_wing=0;

    for(int i=0;i<44;i++){
        amount_npcs[i]=0;
    }

    amount_moving_platforms=0;

    amount_triggers=0;

    amount_doors=0;
    door_local_0=false;
    door_local_1=false;
    door_local_2=false;
    door_local_3=false;
    door_global_0=false;
    door_global_1=false;
    door_global_2=false;
    door_global_3=false;
    door_global_4=false;
    door_global_5=false;
    door_global_6=false;
    door_global_7=false;
    door_global_8=false;
    door_global_9=false;
    door_global_10=false;
    door_global_11=false;

    amount_trap_fire=0;

    WORLD_MAP=false;

    full_brush_w=1;
    full_brush_h=1;

    window_big_x=(camera.w-1440)/2;
    window_big_y=(camera.h-900)/2;
    window_big_moving=false;

    SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/
}

string Brush::return_npc_name(short i){
    if(i==0){
        return "None";
    }
    else if(i==1){
        return "Slime";
    }
    else if(i==2){
        return "Cow";
    }
    else if(i==3){
        return "Bee";
    }
    else if(i==4){
        return "Replicator";
    }
    else if(i==5){
        return "Fish";
    }
    else if(i==6){
        return "Monkey";
    }
    else if(i==7){
        return "Laser spinner";
    }
    else if(i==8){
        return "Penguin";
    }
    else if(i==9){
        return "Fish on ice";
    }
    else if(i==10){
        return "Penguin (harmless)";
    }
    else if(i==11){
        return "Fish on ice (harmless)";
    }
    else if(i==12){
        return "Sword knight";
    }
    else if(i==13){
        return "Ball knight";
    }
    else if(i==14){
        return "Archer";
    }
    else if(i==15){
        return "Scarecrow";
    }
    else if(i==16){
        return "Pumpkin";
    }
    else if(i==17){
        return "Zombie cow";
    }
    else if(i==18){
        return "Mummy cow";
    }
    else if(i==19){
        return "Ghost";
    }
    else if(i==20){
        return "Spider";
    }
    else if(i==21){
        return "Cat";
    }
    else if(i==22){
        return "Walking spider";
    }
    else if(i==23){
        return "Ram";
    }
    else if(i==24){
        return "Eagle";
    }
    else if(i==25){
        return "Snowman";
    }
    else if(i==26){
        return "Fireball";
    }
    else if(i==27){
        return "Dragon whelp";
    }
    else if(i==28){
        return "Fireball shooter left";
    }
    else if(i==29){
        return "Fireball shooter right";
    }
    else if(i==30){
        return "Fireball shooter up";
    }
    else if(i==31){
        return "Fireball shooter down";
    }
    else if(i==32){
        return "Dust devil";
    }
    else if(i==33){
        return "Scorpion";
    }
    else if(i==34){
        return "Sphinx left";
    }
    else if(i==35){
        return "Sphinx right";
    }
    else if(i==36){
        return "Shark";
    }
    else if(i==37){
        return "Henchmouse";
    }
    else if(i==38){
        return "Mechsuit Mouse";
    }
    else if(i==39){
        return "Sally";
    }
    else if(i==40){
        return "Bouncing Cow";
    }
    else if(i==41){
        return "Bouncing Ball";
    }
    else if(i==42){
        return "Sea Turtle";
    }
    else if(i==43){
        return "Starfish";
    }
    else if(i==44){
        return "Boss - Castle";
    }
    else if(i==45){
        return "Boss - Haunted House";
    }
    else if(i==46){
        return "Boss - Volcano";
    }
    else if(i==47){
        return "Boss - Pyramid";
    }
    else if(i==48){
        return "Boss - Lighthouse";
    }
}

void Brush::reset_copy(){
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_solidity[i][n]=-1;
        }
    }
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_sprite[i][n]=-1;
        }
    }
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_special[i][n]=-1;
        }
    }
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_foreground[i][n]=-1;
        }
    }
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_slope[i][n]=-1;
        }
    }
    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            for(short x=0;x<TILE_W;x++){
                copy_height_mask[i][n][x]=-1;
            }
        }
    }

    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_item_type[i][n]=-1;
        }
    }

    for(short i=0;i<MAX_BRUSH_SIZE;i++){
        for(short n=0;n<MAX_BRUSH_SIZE;n++){
            copy_npc_type[i][n]=-1;
        }
    }
}

void Brush::generate_identifiers(short deleted_type,short deleted_identifier){
    //If an object was just deleted.
    //We look through all of the triggers, and if any were targeting this object, we reset their targeting variables.
    if(deleted_identifier!=-1){
        for(int i=0;i<vector_triggers.size();i++){
            for(int j=0;j<vector_triggers[i].targets.size();j++){
                if(vector_triggers[i].targets[j].type==deleted_type && vector_triggers[i].targets[j].identifier==deleted_identifier){
                    vector_triggers[i].targets.erase(vector_triggers[i].targets.begin()+j);
                    j--;
                }
            }
        }
    }

    //If this is not a deletion call.
    //We instead loop through all of the trigger targets, rebuilding the identifiers and ensuring that all triggers keep their current targets.
    else{
        //Moving Platforms.
        short ident=-1;
        for(int i=0;i<vector_moving_platforms.size();i++){
            ident++;

            for(int n=0;n<vector_triggers.size();n++){
                for(int j=0;j<vector_triggers[n].targets.size();j++){
                    if(vector_triggers[n].targets[j].type==0 && vector_triggers[n].targets[j].identifier==vector_moving_platforms[i].identifier){
                        vector_triggers[n].targets[j].identifier=ident;
                    }
                }
            }

            vector_moving_platforms[i].identifier=ident;
        }

        //Doors.
        ident=-1;
        for(int i=0;i<vector_doors.size();i++){
            ident++;

            for(int n=0;n<vector_triggers.size();n++){
                for(int j=0;j<vector_triggers[n].targets.size();j++){
                    if(vector_triggers[n].targets[j].type==1 && vector_triggers[n].targets[j].identifier==vector_doors[i].identifier){
                        vector_triggers[n].targets[j].identifier=ident;
                    }
                }
            }

            vector_doors[i].identifier=ident;
        }

        //Traps.
        ident=-1;
        for(int i=0;i<vector_traps.size();i++){
            ident++;

            for(int n=0;n<vector_triggers.size();n++){
                for(int j=0;j<vector_triggers[n].targets.size();j++){
                    if(vector_triggers[n].targets[j].type==2 && vector_triggers[n].targets[j].identifier==vector_traps[i].identifier){
                        vector_triggers[n].targets[j].identifier=ident;
                    }
                }
            }

            vector_traps[i].identifier=ident;
        }
    }
}

///TEMP
bool Brush::is_tile_ok(int int_x,int int_y){
    if((level.tile_array[int_x][int_y].sprite>=6 && level.tile_array[int_x][int_y].sprite<=18) ||
       (level.tile_array[int_x][int_y].sprite>=41 && level.tile_array[int_x][int_y].sprite<=42) ||
       (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=73)){
        return false;
    }

    return true;
}

void Brush::handle_input(){
    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);
    desert_addition=false;

    if(edit_input==0){
        /**if(keystates[SDLK_o]){
            for(int int_y=0;int_y<(level.level_y/32);int_y++){
                for(int int_x=0;int_x<(level.level_x/32);int_x++){
                    level.tile_array[int_x][int_y].sprite+=2;
                }
            }

            keystates[SDLK_o]=NULL;
        }*/

        ///Use this key for special stuff.
        if(keystates[SDLK_EQUALS]){
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].foreground && ((level.tile_array[int_x][int_y].sprite>=6 && level.tile_array[int_x][int_y].sprite<=14) ||
                       (level.tile_array[int_x][int_y].sprite>=41 && level.tile_array[int_x][int_y].sprite<=42) ||
                       (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=73) ||
                       level.tile_array[int_x][int_y].sprite==16 || level.tile_array[int_x][int_y].sprite==18)){
                        short random=random_range(0,99);
                        if(random<96){
                            level.tile_background_array[int_x][int_y].sprite=1;
                        }
                        else{
                            level.tile_background_array[int_x][int_y].sprite=random_range(2,6);
                        }
                    }
                }
            }

            /**hazard_tiles=0;
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_HAZARD){
                        hazard_tiles++;
                    }
                }
            }*/

            ///desert_addition=true;

            /**for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_background_array[int_x][int_y].sprite==7){
                        level.tile_background_array[int_x][int_y].sprite=1;
                    }
                }
            }*/

            //Reset all necessary sprites.
            /**for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite>=3 && level.tile_array[int_x][int_y].sprite<=5){
                        level.tile_array[int_x][int_y].sprite=0;
                    }
                    else if((level.tile_array[int_x][int_y].sprite>=6 && level.tile_array[int_x][int_y].sprite<=14) ||
                            (level.tile_array[int_x][int_y].sprite>=41 && level.tile_array[int_x][int_y].sprite<=42) ||
                            (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=73) ||
                            level.tile_array[int_x][int_y].sprite==16 || level.tile_array[int_x][int_y].sprite==18){
                        level.tile_array[int_x][int_y].sprite=10;
                    }
                    else if(level.tile_array[int_x][int_y].sprite>=19 && level.tile_array[int_x][int_y].sprite<=22){
                        level.tile_array[int_x][int_y].sprite=19;
                    }
                }
            }
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite==10){
                        level.tile_array[int_x][int_y].sprite=44;
                    }
                    else if(level.tile_array[int_x][int_y].sprite==44){
                        level.tile_array[int_x][int_y].sprite=10;
                    }
                }
            }*/

            /**for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD &&
                       int_y>0 && level.tile_array[int_x][int_y-1].foreground){
                        level.tile_array[int_x][int_y].foreground=true;
                    }
                }
            }*/

            keystates[SDLK_EQUALS]=NULL;
        }

        if(keystates[SDLK_F12]){
            //Reset all necessary sprites.
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite>=3 && level.tile_array[int_x][int_y].sprite<=5){
                        level.tile_array[int_x][int_y].sprite=0;
                    }
                    else if((level.tile_array[int_x][int_y].sprite>=6 && level.tile_array[int_x][int_y].sprite<=14) ||
                            (level.tile_array[int_x][int_y].sprite>=41 && level.tile_array[int_x][int_y].sprite<=42) ||
                            (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=73) ||
                            level.tile_array[int_x][int_y].sprite==16 || level.tile_array[int_x][int_y].sprite==18){
                        level.tile_array[int_x][int_y].sprite=10;
                    }
                    else if(level.tile_array[int_x][int_y].sprite>=19 && level.tile_array[int_x][int_y].sprite<=22){
                        level.tile_array[int_x][int_y].sprite=19;
                    }
                }
            }
            //Add bottoms and tops to ropes.
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite==19 && int_y>0 &&
                       level.tile_array[int_x][int_y-1].special!=TILE_SPECIAL_CLIMBABLE &&
                       level.tile_array[int_x][int_y-1].special!=TILE_SPECIAL_CLIMBABLE_TOP){
                        if(level.tile_array[int_x][int_y-1].sprite==0){
                            level.tile_array[int_x][int_y].sprite=22;
                        }
                    }
                    else if(level.tile_array[int_x][int_y].sprite==19 && int_y<(level.level_y/32)-1 &&
                       level.tile_array[int_x][int_y+1].special!=TILE_SPECIAL_CLIMBABLE &&
                       level.tile_array[int_x][int_y+1].special!=TILE_SPECIAL_CLIMBABLE_TOP){
                        if(level.tile_array[int_x][int_y+1].sprite==0){
                            level.tile_array[int_x][int_y].sprite=20;
                        }
                        else if(level.tile_array[int_x][int_y+1].solidity==TILE_SOLIDITY_SOLID){
                            level.tile_array[int_x][int_y].sprite=21;
                        }
                    }
                }
            }
            //Add hanging clutter.
            for(int int_y=0;int_y<(level.level_y/32);int_y++){
                for(int int_x=0;int_x<(level.level_x/32);int_x++){
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE &&
                       (level.tile_array[int_x][int_y].sprite==0 || (level.tile_array[int_x][int_y].sprite>=28 && level.tile_array[int_x][int_y].sprite<=29))){
                        level.tile_array[int_x][int_y].sprite=0;
                        level.tile_array[int_x][int_y].foreground=false;

                        if(int_y>0 && level.tile_array[int_x][int_y-1].sprite==10){
                            short random=random_range(0,99);
                            if(random<12){
                                level.tile_array[int_x][int_y].sprite=random_range(28,29);
                                level.tile_array[int_x][int_y].foreground=true;
                            }
                        }
                    }
                }
            }
            //Set ground edges.
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite==10 ||
                            (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=66)){
                        bool blank_up=false;
                        bool blank_down=false;
                        bool blank_left=false;
                        bool blank_right=false;

                        if(int_y>0 && is_tile_ok(int_x,int_y-1)){
                            blank_up=true;
                        }
                        if(int_y<(level.level_y/32)-1 && is_tile_ok(int_x,int_y+1)){
                            blank_down=true;
                        }
                        if(int_x>0 && is_tile_ok(int_x-1,int_y)){
                            blank_left=true;
                        }
                        if(int_x<(level.level_x/32)-1 && is_tile_ok(int_x+1,int_y)){
                            blank_right=true;
                        }
                        if(blank_right && !(blank_up || blank_left || blank_down)){
                            level.tile_array[int_x][int_y].sprite=11;
                        }
                        if(blank_left && !(blank_up || blank_right || blank_down)){
                            level.tile_array[int_x][int_y].sprite=9;
                        }
                        if(blank_down && !(blank_up || blank_left || blank_right)){
                            level.tile_array[int_x][int_y].sprite=13;
                        }
                        if(blank_up && !(blank_right || blank_left || blank_down)){
                            level.tile_array[int_x][int_y].sprite=7;
                        }
                        if(blank_up && blank_left && !(blank_right || blank_down)){
                            level.tile_array[int_x][int_y].sprite=6;
                        }
                        if(blank_up && blank_right && !(blank_left || blank_down)){
                            level.tile_array[int_x][int_y].sprite=8;
                        }
                        if(blank_down && blank_left && !(blank_right || blank_up)){
                            level.tile_array[int_x][int_y].sprite=12;
                        }
                        if(blank_down && blank_right && !(blank_left || blank_up)){
                            level.tile_array[int_x][int_y].sprite=14;
                        }
                        if(blank_down && blank_up && !(blank_left || blank_right)){
                            level.tile_array[int_x][int_y].sprite=67;
                        }
                        if(blank_left && blank_right && !(blank_up || blank_down)){
                            level.tile_array[int_x][int_y].sprite=68;
                        }
                        if(blank_down && blank_up && blank_right && !blank_left){
                            level.tile_array[int_x][int_y].sprite=69;
                        }
                        if(blank_down && blank_up && blank_left && !blank_right){
                            level.tile_array[int_x][int_y].sprite=70;
                        }
                        if(blank_left && blank_up && blank_right && !blank_down){
                            level.tile_array[int_x][int_y].sprite=71;
                        }
                        if(blank_left && blank_down && blank_right && !blank_up){
                            level.tile_array[int_x][int_y].sprite=72;
                        }
                        if(blank_left && blank_up && blank_right && blank_down){
                            level.tile_array[int_x][int_y].sprite=73;
                        }
                    }
                }
            }
            //Add ground tops.
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(((level.tile_array[int_x][int_y].sprite>=6 && level.tile_array[int_x][int_y].sprite<=8) ||
                        level.tile_array[int_x][int_y].sprite==67 || level.tile_array[int_x][int_y].sprite==73 ||
                       (level.tile_array[int_x][int_y].sprite>=69 && level.tile_array[int_x][int_y].sprite<=71)) && int_y>0){
                        if(level.tile_array[int_x][int_y-1].sprite==0 && level.tile_array[int_x][int_y-1].special==TILE_SPECIAL_NONE){
                            level.tile_array[int_x][int_y-1].sprite=4;
                        }
                    }
                }
            }
            //Add ground clutter.
            for(int int_y=0;int_y<(level.level_y/32);int_y++){
                for(int int_x=0;int_x<(level.level_x/32);int_x++){
                    if(level.tile_array[int_x][int_y].sprite==4){
                        short random=random_range(0,99);
                        if(random<12){
                            if(random_range(0,1)==0){
                                level.tile_array[int_x][int_y].sprite=3;
                            }
                            else{
                                level.tile_array[int_x][int_y].sprite=5;
                            }
                        }
                    }
                }
            }
            //Handle slopes.
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite==15){
                        if(int_x>0 && int_y>0 && level.tile_array[int_x-1][int_y-1].sprite!=15){
                            if(level.tile_array[int_x-1][int_y].sprite==10){
                                level.tile_array[int_x-1][int_y].sprite=41;
                            }
                        }
                        if(int_x<(level.level_x/32)-1 && int_y<(level.level_y/32)-1 && level.tile_array[int_x+1][int_y+1].sprite==15){
                            level.tile_array[int_x][int_y+1].sprite=16;
                        }
                    }
                }
            }
            for(int int_x=0;int_x<level.level_x/32;int_x++){
                for(int int_y=0;int_y<level.level_y/32;int_y++){
                    if(level.tile_array[int_x][int_y].sprite==17){
                        if(int_x<(level.level_x/32)-1 && int_y>0 && level.tile_array[int_x+1][int_y-1].sprite!=17){
                            if(level.tile_array[int_x+1][int_y].sprite==10){
                                level.tile_array[int_x+1][int_y].sprite=42;
                            }
                        }
                        if(int_x>0 && int_y<(level.level_y/32)-1 && level.tile_array[int_x-1][int_y+1].sprite==17){
                            level.tile_array[int_x][int_y+1].sprite=18;
                        }
                    }
                }
            }
            //Add tile and background clutter.
            for(int int_y=0;int_y<(level.level_y/32);int_y++){
                for(int int_x=0;int_x<(level.level_x/32);int_x++){
                    if(level.tile_array[int_x][int_y].sprite==10 || (level.tile_array[int_x][int_y].sprite>=62 && level.tile_array[int_x][int_y].sprite<=66)){
                        short random=random_range(0,99);
                        if(random<96){
                            level.tile_array[int_x][int_y].sprite=10;
                        }
                        else{
                            level.tile_array[int_x][int_y].sprite=random_range(62,66);
                        }
                    }
                    if(level.tile_background_array[int_x][int_y].sprite>=1 && level.tile_background_array[int_x][int_y].sprite<=6){
                        short random=random_range(0,99);
                        if(random<96){
                            level.tile_background_array[int_x][int_y].sprite=1;
                        }
                        else{
                            level.tile_background_array[int_x][int_y].sprite=random_range(2,6);
                        }
                    }
                }
            }

            keystates[SDLK_F12]=NULL;
        }

        //If the left directional key is pressed.
        if(keystates[SDLK_LEFT]){
            //Set their move state to match the directional key.
            move_state=LEFT;
        }

        //If the up directional key is pressed.
        if(keystates[SDLK_UP]){
            move_state=UP;
        }

        //If the right directional key is pressed.
        if(keystates[SDLK_RIGHT]){
            move_state=RIGHT;
        }

        //If the down directional key is pressed.
        if(keystates[SDLK_DOWN]){
            move_state=DOWN;
        }

        if(keystates[SDLK_KP1]){
            move_state=LEFT;
        }
        if(keystates[SDLK_KP5]){
            move_state=UP;
        }
        if(keystates[SDLK_KP3]){
            move_state=RIGHT;
        }
        if(keystates[SDLK_KP2]){
            move_state=DOWN;
        }

        if(keystates[SDLK_KP1] && keystates[SDLK_KP5]){
            move_state=LEFT_UP;
        }
        if(keystates[SDLK_KP5] && keystates[SDLK_KP3]){
            move_state=RIGHT_UP;
        }
        if(keystates[SDLK_KP3] && keystates[SDLK_KP2]){
            move_state=RIGHT_DOWN;
        }
        if(keystates[SDLK_KP2] && keystates[SDLK_KP1]){
            move_state=LEFT_DOWN;
        }
        if(keystates[SDLK_KP1] && keystates[SDLK_KP3]){
            move_state=LEFT;
        }
        if(keystates[SDLK_KP5] && keystates[SDLK_KP2]){
            move_state=UP;
        }
        if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP3]){
            move_state=LEFT_UP;
        }
        if(keystates[SDLK_KP1] && keystates[SDLK_KP2] && keystates[SDLK_KP3]){
            move_state=LEFT_DOWN;
        }
        if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP2]){
            move_state=LEFT_UP;
        }
        if(keystates[SDLK_KP5] && keystates[SDLK_KP3] && keystates[SDLK_KP2]){
            move_state=RIGHT_UP;
        }
        if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP3] && keystates[SDLK_KP2]){
            move_state=LEFT_UP;
        }

        //If no directional keys are pressed, the player is not moving.
        if(!keystates[SDLK_LEFT] && !keystates[SDLK_UP] && !keystates[SDLK_RIGHT] && !keystates[SDLK_DOWN] && !keystates[SDLK_KP1] && !keystates[SDLK_KP2] && !keystates[SDLK_KP3] && !keystates[SDLK_KP5]){
            move_state=0;
        }

        if(!keystates[SDLK_RCTRL] && !keystates[SDLK_LCTRL]){
            keystates[SDLK_LEFT]=NULL;
            keystates[SDLK_UP]=NULL;
            keystates[SDLK_RIGHT]=NULL;
            keystates[SDLK_DOWN]=NULL;
        }

        //Toggle current layer.
        if(keystates[SDLK_l] || keystates[SDLK_k]){
            if(keystates[SDLK_l]){
                layer_current++;
                if(layer_current>10){
                    layer_current=0;
                }
            }
            else if(keystates[SDLK_k]){
                layer_current--;
                if(layer_current<0){
                    layer_current=10;
                }
            }

            reset_copy();

            current_platform=-1;
            current_trigger=-1;
            current_door=-1;
            current_trap=-1;
            current_sign=-1;
            current_spring=-1;
            current_booster=-1;

            full_brush_w=1;
            full_brush_h=1;

            //Tiles layer.
            if(layer_current==0){
                w=32;
                h=32;
                for(int int_y=0;int_y<(level.level_y/32);int_y++){
                    for(int int_x=0;int_x<(level.level_x/32);int_x++){
                        if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_W,TILE_H)){
                            x=level.tile_array[int_x][int_y].x;
                            y=level.tile_array[int_x][int_y].y;
                        }
                    }
                }
            }
            //Items layer.
            else if(layer_current==1){
                w=ITEM_SIZE;
                h=ITEM_SIZE;
            }
            //Background tiles layer.
            if(layer_current==2){
                w=32;
                h=32;
                for(int int_y=0;int_y<(level.level_y/32);int_y++){
                    for(int int_x=0;int_x<(level.level_x/32);int_x++){
                        if(collision_check(x,y,w,h,level.tile_background_array[int_x][int_y].x,level.tile_background_array[int_x][int_y].y,TILE_W,TILE_H)){
                            x=level.tile_background_array[int_x][int_y].x;
                            y=level.tile_background_array[int_x][int_y].y;
                        }
                    }
                }
            }
            //Npcs layer.
            else if(layer_current==3){
                w=NPC_SIZE;
                h=NPC_SIZE;
                for(int i=0;i<vector_npcs.size();i++){
                    if(collision_check(x,y,w,h,vector_npcs.at(i).x,vector_npcs.at(i).y,vector_npcs.at(i).w,vector_npcs.at(i).h)){
                        x=vector_npcs.at(i).x;
                        y=vector_npcs.at(i).y;
                    }
                }
            }
            //Moving platforms layer.
            else if(layer_current==4){
                w=MOVING_PLATFORM_SIZE;
                h=MOVING_PLATFORM_SIZE;
                for(int i=0;i<vector_moving_platforms.size();i++){
                    if(collision_check(x,y,w,h,vector_moving_platforms.at(i).x,vector_moving_platforms.at(i).y,vector_moving_platforms.at(i).w,vector_moving_platforms.at(i).h)){
                        x=vector_moving_platforms.at(i).x;
                        y=vector_moving_platforms.at(i).y;
                    }
                }
            }
            //Triggers layer.
            else if(layer_current==5){
                w=TRIGGERS_SIZE;
                h=TRIGGERS_SIZE;
                x=(int)((int)x/TRIGGERS_SIZE)*TRIGGERS_SIZE;
                y=(int)((int)y/TRIGGERS_SIZE)*TRIGGERS_SIZE;
            }
            //Doors layer.
            else if(layer_current==6){
                w=DOOR_SIZE;
                h=DOOR_SIZE;
                x=(int)((int)x/DOOR_SIZE)*DOOR_SIZE;
                y=(int)((int)y/DOOR_SIZE)*DOOR_SIZE;
            }
            //Traps layer.
            else if(layer_current==7){
                w=TRAP_SIZE;
                h=TRAP_SIZE;
                x=(int)((int)x/TRAP_SIZE)*TRAP_SIZE;
                y=(int)((int)y/TRAP_SIZE)*TRAP_SIZE;
            }
            //Signs layer.
            else if(layer_current==8){
                w=SIGN_SIZE;
                h=SIGN_SIZE;
                x=(int)((int)x/SIGN_SIZE)*SIGN_SIZE;
                y=(int)((int)y/SIGN_SIZE)*SIGN_SIZE;
            }
            //Springs layer.
            else if(layer_current==9){
                w=SPRING_SIZE;
                h=SPRING_SIZE;
                x=(int)((int)x/SPRING_SIZE)*SPRING_SIZE;
                y=(int)((int)y/SPRING_SIZE)*SPRING_SIZE;
            }
            //Boosters layer.
            else if(layer_current==10){
                w=BOOSTER_SIZE;
                h=BOOSTER_SIZE;
                x=(int)((int)x/BOOSTER_SIZE)*BOOSTER_SIZE;
                y=(int)((int)y/BOOSTER_SIZE)*BOOSTER_SIZE;
            }

            keystates[SDLK_l]=NULL;
            keystates[SDLK_k]=NULL;
        }

        //As long as we aren't editing the moving platform, trigger, door, or trap layer.
        if(layer_current!=4 && layer_current!=5 && layer_current!=6 && layer_current!=7 && layer_current!=8){
            //Increase/decrease brush size.
            if(keystates[SDLK_KP_MULTIPLY]){
                if(full_brush_w<MAX_BRUSH_SIZE){
                    full_brush_w++;
                }
                reset_copy();

                keystates[SDLK_KP_MULTIPLY]=NULL;
            }
            if(keystates[SDLK_KP_DIVIDE]){
                if(full_brush_w>1){
                    full_brush_w--;
                }
                reset_copy();

                keystates[SDLK_KP_DIVIDE]=NULL;
            }
            if(keystates[SDLK_KP_PLUS]){
                if(full_brush_h<MAX_BRUSH_SIZE){
                    full_brush_h++;
                }
                reset_copy();

                keystates[SDLK_KP_PLUS]=NULL;
            }
            if(keystates[SDLK_KP_MINUS]){
                if(full_brush_h>1){
                    full_brush_h--;
                }
                reset_copy();

                keystates[SDLK_KP_MINUS]=NULL;
            }
        }

        ///I need to add a menu for all of these layer display toggles.

        //Toggle display of tiles layer.
        /**if(keystates[SDLK_KP7]){
            layer_tiles=!layer_tiles;

            keystates[SDLK_KP7]=NULL;
        }

        //Toggle display of items layer.
        if(keystates[SDLK_KP8]){
            layer_items=!layer_items;

            keystates[SDLK_KP8]=NULL;
        }

        //Toggle display of background tiles layer.
        if(keystates[SDLK_KP9]){
            layer_tiles_background=!layer_tiles_background;

            keystates[SDLK_KP9]=NULL;
        }

        //Toggle display of npcs layer.
        if(keystates[SDLK_KP4]){
            layer_npcs=!layer_npcs;

            keystates[SDLK_KP4]=NULL;
        }*/

        //Toggle speed mode.
        if(keystates[SDLK_KP7]){
            speed_mode=!speed_mode;

            keystates[SDLK_KP7]=NULL;
        }

        //Toggle world map mode.
        if(keystates[SDLK_w] && layer_current!=4 && layer_current!=5 && layer_current!=6 && layer_current!=7){
            WORLD_MAP=!WORLD_MAP;

            item_current=0;

            keystates[SDLK_w]=NULL;
        }

        //**********************************//
        // Editing the terrain tiles layer: //
        //**********************************//

        if(layer_current==0){
            //Edit sprite.
            if(keystates[SDLK_p]){
                edit_input=4;
                edit_string="";
                SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                keystates[SDLK_p]=NULL;
            }

            //Edit sloping ground angle.
            if(keystates[SDLK_a]){
                edit_input=11;
                edit_string="";
                SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                keystates[SDLK_a]=NULL;
            }

            //Edit height mask.
            if(keystates[SDLK_h]){
                edit_input=15;
                edit_string="";
                SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                keystates[SDLK_h]=NULL;
            }

            //Toggle solidity.
            if(keystates[SDLK_d]){
                for(short i=0;i<full_brush_w;i++){
                    for(short n=0;n<full_brush_h;n++){
                        //Set the brush's current tile position.
                        int brush_current_x=(int)((int)(x+w*i)/w);
                        int brush_current_y=(int)((int)(y+h*n)/h);

                        level.tile_array[brush_current_x][brush_current_y].solidity++;
                        if(level.tile_array[brush_current_x][brush_current_y].solidity>2){
                            level.tile_array[brush_current_x][brush_current_y].solidity=0;
                        }
                    }
                }

                keystates[SDLK_d]=NULL;
            }

            //Toggle special.
            if(keystates[SDLK_g]){
                for(short i=0;i<full_brush_w;i++){
                    for(short n=0;n<full_brush_h;n++){
                        //Set the brush's current tile position.
                        int brush_current_x=(int)((int)(x+w*i)/w);
                        int brush_current_y=(int)((int)(y+h*n)/h);

                        level.tile_array[brush_current_x][brush_current_y].special++;
                        if(level.tile_array[brush_current_x][brush_current_y].special>6){
                            level.tile_array[brush_current_x][brush_current_y].special=0;
                        }
                    }
                }

                keystates[SDLK_g]=NULL;
            }

            //Toggle foreground.
            if(keystates[SDLK_f]){
                for(short i=0;i<full_brush_w;i++){
                    for(short n=0;n<full_brush_h;n++){
                        //Set the brush's current tile position.
                        int brush_current_x=(int)((int)(x+w*i)/w);
                        int brush_current_y=(int)((int)(y+h*n)/h);

                        level.tile_array[brush_current_x][brush_current_y].foreground=!level.tile_array[brush_current_x][brush_current_y].foreground;
                    }
                }

                keystates[SDLK_f]=NULL;
            }
        }

        //*************************************//
        // Editing the background tiles layer: //
        //*************************************//

        else if(layer_current==2){
            //Edit sprite.
            if(keystates[SDLK_p]){
                edit_input=4;
                edit_string="";
                SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                keystates[SDLK_p]=NULL;
            }
        }

        //**************************//
        // Editing the items layer: //
        //**************************//

        else if(layer_current==1){
            //Place item.
            if(keystates[SDLK_SPACE] && (WORLD_MAP || (!WORLD_MAP && item_current!=0))){
                for(short i=0;i<full_brush_w;i++){
                    for(short n=0;n<full_brush_h;n++){
                        create_item(i,n,item_current);
                    }
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select item for editing.
            if(keystates[SDLK_RETURN]){
                bool item_here=false;
                for(int j=0;j<vector_items.size();j++){
                    if(collision_check(x,y,w,h,vector_items.at(j).x,vector_items.at(j).y,vector_items.at(j).w,vector_items.at(j).h)){
                        if(j!=item_selected){
                            item_selected=j;
                            item_here=true;
                        }
                    }
                }

                if(!item_here){
                    item_selected=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //As long as there is currently a selected item.
            if(item_selected!=-1){
                //Edit goal_level_to_load.
                if(keystates[SDLK_g]){
                    edit_input=19;
                    edit_string="";
                    //Enable unicode.
                    SDL_EnableUNICODE(SDL_ENABLE);

                    keystates[SDLK_g]=NULL;
                }

                //Toggle goal_secret.
                if(keystates[SDLK_s]){
                    vector_items[item_selected].goal_secret=!vector_items[item_selected].goal_secret;

                    keystates[SDLK_s]=NULL;
                }
            }
        }

        //*************************//
        // Editing the npcs layer: //
        //*************************//

        else if(layer_current==3){
            //Place npc.
            if(keystates[SDLK_SPACE] && npc_current!=0){
                for(short i=0;i<full_brush_w;i++){
                    for(short n=0;n<full_brush_h;n++){
                        create_npc(i,n,npc_current);
                    }
                }

                keystates[SDLK_SPACE]=NULL;
            }
        }

        //*************************************//
        // Editing the moving platforms layer: //
        //*************************************//

        else if(layer_current==4){
            //Place moving platform starting point.
            if(keystates[SDLK_SPACE]){
                bool platform_here=false;
                for(int j=0;j<vector_moving_platforms.size();j++){
                    if(collision_check(x,y,w,h,vector_moving_platforms[j].x,vector_moving_platforms[j].y,vector_moving_platforms[j].w,vector_moving_platforms[j].h)){
                        platform_here=true;
                        break;
                    }
                }
                if(!platform_here){
                    vector<moving_platform_waypoint> temp_vector;
                    temp_vector.clear();

                    vector_moving_platforms.push_back(Moving_Platform(1.0,x,y,0,0,temp_vector,true,true,false,-1));

                    current_platform=vector_moving_platforms.size()-1;

                    generate_identifiers();
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select platform for end point/waypoint editing.
            if(keystates[SDLK_RETURN]){
                bool platform_here=false;
                for(int j=0;j<vector_moving_platforms.size();j++){
                    if(collision_check(x,y,w,h,vector_moving_platforms.at(j).x,vector_moving_platforms.at(j).y,vector_moving_platforms.at(j).w,vector_moving_platforms.at(j).h)){
                        if(j!=current_platform){
                            current_platform=j;
                            platform_here=true;
                        }
                    }
                }

                if(!platform_here){
                    current_platform=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing platforms.
            if(keystates[SDLK_EQUALS]){
                //Check through the platforms until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_moving_platforms.size();i++){
                    //If we have found an existing platform.
                    if(i>current_platform){
                        //
                        current_platform=i;

                        //Snap the brush to the platform's location.
                        x=vector_moving_platforms[i].start_point_x;
                        y=vector_moving_platforms[i].start_point_y;

                        //Stop looping through the platforms.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing platforms.
            if(keystates[SDLK_MINUS]){
                //
                if(vector_moving_platforms.size()>0 && current_platform==-1){
                    current_platform=vector_moving_platforms.size()-1;
                }

                //Check through the platforms until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_moving_platforms.size()-1;i>=0;i--){
                    //If we have found an existing platform.
                    if(i<current_platform){
                        //
                        current_platform=i;

                        //Snap the brush to the platform's location.
                        x=vector_moving_platforms[i].start_point_x;
                        y=vector_moving_platforms[i].start_point_y;

                        //Stop looping through the platforms.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected platform.
            if(current_platform!=-1){
                //Edit move_speed.
                if(keystates[SDLK_s]){
                    edit_input=1;
                    edit_string="";
                    //Enable unicode.
                    SDL_EnableUNICODE(SDL_ENABLE);

                    keystates[SDLK_s]=NULL;
                }

                //Jump to End Point.
                if(keystates[SDLK_j]){
                    x=vector_moving_platforms[current_platform].end_point_x;
                    y=vector_moving_platforms[current_platform].end_point_y;

                    keystates[SDLK_j]=NULL;
                }

                //Place End Point.
                if(keystates[SDLK_e]){
                    vector_moving_platforms[current_platform].end_point_x=x;
                    vector_moving_platforms[current_platform].end_point_y=y;

                    keystates[SDLK_e]=NULL;
                }

                //Place Start Point.
                if(keystates[SDLK_p]){
                    bool platform_here=false;
                    for(int j=0;j<vector_moving_platforms.size();j++){
                        if(collision_check(x,y,w,h,vector_moving_platforms[j].x,vector_moving_platforms[j].y,vector_moving_platforms[j].w,vector_moving_platforms[j].h)){
                            platform_here=true;
                            break;
                        }
                    }
                    if(!platform_here){
                        vector_moving_platforms[current_platform].start_point_x=x;
                        vector_moving_platforms[current_platform].start_point_y=y;
                        vector_moving_platforms[current_platform].x=x;
                        vector_moving_platforms[current_platform].y=y;
                    }

                    keystates[SDLK_p]=NULL;
                }

                //Place Waypoint.
                if(keystates[SDLK_w]){
                    //As long as there is not already a waypoint here.
                    for(int i=0;i<vector_moving_platforms[current_platform].waypoints.size();i++){
                        if(vector_moving_platforms[current_platform].waypoints[i].x==x && vector_moving_platforms[current_platform].waypoints[i].y==y){
                            break;
                        }

                        if(i==vector_moving_platforms[current_platform].waypoints.size()-1){
                            vector_moving_platforms[current_platform].waypoints.push_back(moving_platform_waypoint());
                            vector_moving_platforms[current_platform].waypoints.back().x=x;
                            vector_moving_platforms[current_platform].waypoints.back().y=y;
                            break;
                        }
                    }
                    if(vector_moving_platforms[current_platform].waypoints.size()==0){
                        vector_moving_platforms[current_platform].waypoints.push_back(moving_platform_waypoint());
                        vector_moving_platforms[current_platform].waypoints.back().x=x;
                        vector_moving_platforms[current_platform].waypoints.back().y=y;
                    }

                    keystates[SDLK_w]=NULL;
                }

                //Remove Waypoint.
                if(keystates[SDLK_q]){
                    //
                    if(vector_moving_platforms[current_platform].waypoints.size()>0){
                        vector_moving_platforms[current_platform].waypoints.pop_back();
                    }

                    keystates[SDLK_q]=NULL;
                }

                //Toggle active.
                if(keystates[SDLK_a]){
                    vector_moving_platforms[current_platform].active=!vector_moving_platforms[current_platform].active;

                    keystates[SDLK_a]=NULL;
                }

                //Toggle round_trip.
                if(keystates[SDLK_r]){
                    vector_moving_platforms[current_platform].round_trip=!vector_moving_platforms[current_platform].round_trip;

                    keystates[SDLK_r]=NULL;
                }

                //Toggle movement_type.
                if(keystates[SDLK_m]){
                    vector_moving_platforms[current_platform].movement_type=!vector_moving_platforms[current_platform].movement_type;

                    keystates[SDLK_m]=NULL;
                }
            }
        }

        //*****************************//
        // Editing the triggers layer: //
        //*****************************//

        else if(layer_current==5){
            //Place trigger.
            if(keystates[SDLK_SPACE]){
                bool trigger_here=false;
                for(int j=0;j<vector_triggers.size();j++){
                    if(collision_check(x,y,w,h,vector_triggers[j].x,vector_triggers[j].y,vector_triggers[j].w,vector_triggers[j].h)){
                        trigger_here=true;
                        break;
                    }
                }

                if(!trigger_here){
                    vector<target> targets;
                    targets.clear();

                    vector_triggers.push_back(Trigger(x,y,32,32,targets,false,0,true,5,0));

                    current_trigger=vector_triggers.size()-1;
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select trigger for editing.
            if(keystates[SDLK_RETURN]){
                bool trigger_here=false;
                for(int j=0;j<vector_triggers.size();j++){
                    if(collision_check(x,y,w,h,vector_triggers.at(j).x,vector_triggers.at(j).y,vector_triggers.at(j).w,vector_triggers.at(j).h)){
                        if(j!=current_trigger){
                            current_trigger=j;
                            trigger_here=true;
                        }
                    }
                }

                if(!trigger_here){
                    current_trigger=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing triggers.
            if(keystates[SDLK_EQUALS]){
                //Check through the triggers until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_triggers.size();i++){
                    //If we have found an existing trigger.
                    if(i>current_trigger){
                        //
                        current_trigger=i;

                        //Snap the brush to the trigger's location.
                        x=vector_triggers[i].x;
                        y=vector_triggers[i].y;

                        //Stop looping through the triggers.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing triggers.
            if(keystates[SDLK_MINUS]){
                //
                if(vector_triggers.size()>0 && current_trigger==-1){
                    current_trigger=vector_triggers.size()-1;
                }

                //Check through the triggers until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_triggers.size()-1;i>=0;i--){
                    //If we have found an existing trigger.
                    if(i<current_trigger){
                        //
                        current_trigger=i;

                        //Snap the brush to the trigger's location.
                        x=vector_triggers[i].x;
                        y=vector_triggers[i].y;

                        //Stop looping through the platforms.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected trigger.
            if(current_trigger!=-1){
                //Edit repeat_time.
                if(keystates[SDLK_p]){
                    edit_input=13;
                    edit_string="";
                    //Enable unicode.
                    SDL_EnableUNICODE(SDL_ENABLE);

                    keystates[SDLK_p]=NULL;
                }

                //Move Trigger.
                if(keystates[SDLK_m]){
                    bool trigger_here=false;
                    for(int j=0;j<vector_triggers.size();j++){
                        if(collision_check(x,y,w,h,vector_triggers[j].x,vector_triggers[j].y,vector_triggers[j].w,vector_triggers[j].h)){
                            trigger_here=true;
                            break;
                        }
                    }
                    if(!trigger_here){
                        vector_triggers[current_trigger].x=x;
                        vector_triggers[current_trigger].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Set target.
                if(keystates[SDLK_RSHIFT]){
                    bool target_selected=false;

                    //Used to check and see if the trigger is already targeting whatever has just been selected.
                    bool targeting_this;

                    if(!target_selected){
                        for(int i=0;i<vector_moving_platforms.size();i++){
                            //If we have selected this object.
                            if(collision_check(x,y,w,h,vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h)){
                                targeting_this=false;

                                //Check to see if this object is already targeted by the current trigger.
                                for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                    if(vector_triggers[current_trigger].targets[n].type==0 && vector_triggers[current_trigger].targets[n].identifier==vector_moving_platforms[i].identifier){
                                        targeting_this=true;
                                    }
                                }

                                //If we aren't already targeting this object.
                                if(!targeting_this){
                                    vector_triggers[current_trigger].targets.push_back(target());

                                    //This trigger targets a moving platform.
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].type=0;

                                    //
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].identifier=vector_moving_platforms[i].identifier;

                                    target_selected=true;
                                }

                                break;
                            }
                        }
                    }
                    if(!target_selected){
                        for(int i=0;i<vector_doors.size();i++){
                            //If we have selected this object.
                            if(collision_check(x,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                                targeting_this=false;

                                //Check to see if this object is already targeted by the current trigger.
                                for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                    if(vector_triggers[current_trigger].targets[n].type==1 && vector_triggers[current_trigger].targets[n].identifier==vector_doors[i].identifier){
                                        targeting_this=true;
                                    }
                                }

                                //If we aren't already targeting this object.
                                if(!targeting_this){
                                    vector_triggers[current_trigger].targets.push_back(target());

                                    //This trigger targets a door.
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].type=1;

                                    //
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].identifier=vector_doors[i].identifier;

                                    target_selected=true;
                                }

                                break;
                            }
                        }
                    }
                    if(!target_selected){
                        for(int i=0;i<vector_traps.size();i++){
                            //If we have selected this object.
                            if(collision_check(x,y,w,h,vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                                targeting_this=false;

                                //Check to see if this object is already targeted by the current trigger.
                                for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                    if(vector_triggers[current_trigger].targets[n].type==2 && vector_triggers[current_trigger].targets[n].identifier==vector_traps[i].identifier){
                                        targeting_this=true;
                                    }
                                }

                                //If we aren't already targeting this object.
                                if(!targeting_this){
                                    vector_triggers[current_trigger].targets.push_back(target());

                                    //This trigger targets a trap.
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].type=2;

                                    //
                                    vector_triggers[current_trigger].targets[vector_triggers[current_trigger].targets.size()-1].identifier=vector_traps[i].identifier;

                                    target_selected=true;
                                }

                                break;
                            }
                        }
                    }

                    keystates[SDLK_RSHIFT]=NULL;
                }

                //Remove target.
                if(keystates[SDLK_SLASH]){
                    for(int i=0;i<vector_moving_platforms.size();i++){
                        //If we have selected this object.
                        if(collision_check(x,y,w,h,vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h)){
                            //Check to see if this object is targeted by the current trigger.
                            for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                //If this object is targeted by the current trigger.
                                if(vector_triggers[current_trigger].targets[n].type==0 && vector_triggers[current_trigger].targets[n].identifier==vector_moving_platforms[i].identifier){
                                    //Untarget this object.
                                    vector_triggers[current_trigger].targets.erase(vector_triggers[current_trigger].targets.begin()+n);
                                }
                            }
                        }
                    }

                    for(int i=0;i<vector_doors.size();i++){
                        //If we have selected this object.
                        if(collision_check(x,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                            //Check to see if this object is targeted by the current trigger.
                            for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                //If this object is targeted by the current trigger.
                                if(vector_triggers[current_trigger].targets[n].type==1 && vector_triggers[current_trigger].targets[n].identifier==vector_doors[i].identifier){
                                    //Untarget this object.
                                    vector_triggers[current_trigger].targets.erase(vector_triggers[current_trigger].targets.begin()+n);
                                }
                            }
                        }
                    }

                    for(int i=0;i<vector_traps.size();i++){
                        //If we have selected this object.
                        if(collision_check(x,y,w,h,vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                            //Check to see if this object is targeted by the current trigger.
                            for(int n=0;n<vector_triggers[current_trigger].targets.size();n++){
                                //If this object is targeted by the current trigger.
                                if(vector_triggers[current_trigger].targets[n].type==2 && vector_triggers[current_trigger].targets[n].identifier==vector_traps[i].identifier){
                                    //Untarget this object.
                                    vector_triggers[current_trigger].targets.erase(vector_triggers[current_trigger].targets.begin()+n);
                                }
                            }
                        }
                    }

                    keystates[SDLK_SLASH]=NULL;
                }

                //Jump to Target.
                /**if(keystates[SDLK_j]){
                    //If this trigger has a target selected.
                    if(vector_triggers[current_trigger].target_identifier!=-1){
                        //If the target is a moving platform.
                        if(vector_triggers[current_trigger].target_type==0){
                            for(int i=0;i<vector_moving_platforms.size();i++){
                                if(vector_moving_platforms[i].identifier==vector_triggers[current_trigger].target_identifier){
                                    x=vector_moving_platforms[i].x;
                                    y=vector_moving_platforms[i].y;
                                    break;
                                }
                            }
                        }
                    }

                    keystates[SDLK_j]=NULL;
                }*/

                //Increase width.
                if(keystates[SDLK_KP_MULTIPLY]){
                    vector_triggers[current_trigger].w+=32;

                    keystates[SDLK_KP_MULTIPLY]=NULL;
                }

                //Decrease width.
                if(keystates[SDLK_KP_DIVIDE]){
                    vector_triggers[current_trigger].w-=32;
                    if(vector_triggers[current_trigger].w<32){
                        vector_triggers[current_trigger].w=32;
                    }

                    keystates[SDLK_KP_DIVIDE]=NULL;
                }

                //Increase height.
                if(keystates[SDLK_KP_PLUS]){
                    vector_triggers[current_trigger].h+=32;

                    keystates[SDLK_KP_PLUS]=NULL;
                }

                //Decrease height.
                if(keystates[SDLK_KP_MINUS]){
                    vector_triggers[current_trigger].h-=32;
                    if(vector_triggers[current_trigger].h<32){
                        vector_triggers[current_trigger].h=32;
                    }

                    keystates[SDLK_KP_MINUS]=NULL;
                }

                //Toggle trigger_method.
                if(keystates[SDLK_t]){
                    vector_triggers[current_trigger].trigger_method=!vector_triggers[current_trigger].trigger_method;

                    keystates[SDLK_t]=NULL;
                }

                //Toggle user_type.
                if(keystates[SDLK_u]){
                    vector_triggers[current_trigger].user_type++;
                    if(vector_triggers[current_trigger].user_type>1){
                        vector_triggers[current_trigger].user_type=0;
                    }

                    keystates[SDLK_u]=NULL;
                }

                //Toggle repeating.
                if(keystates[SDLK_r]){
                    vector_triggers[current_trigger].repeating=!vector_triggers[current_trigger].repeating;

                    keystates[SDLK_r]=NULL;
                }

                //Toggle render.
                if(keystates[SDLK_e]){
                    vector_triggers[current_trigger].render_trigger++;
                    if(vector_triggers[current_trigger].render_trigger>5){
                        vector_triggers[current_trigger].render_trigger=0;
                    }

                    keystates[SDLK_e]=NULL;
                }
            }
        }

        //**************************//
        // Editing the doors layer: //
        //**************************//

        else if(layer_current==6){
            //Create door.
            if(keystates[SDLK_SPACE]){
                bool door_here=false;
                for(int j=0;j<vector_doors.size();j++){
                    if(collision_check(x,y,w,h,vector_doors[j].x,vector_doors[j].y,vector_doors[j].w,vector_doors[j].h)){
                        door_here=true;
                        break;
                    }
                }
                if(!door_here){
                    vector_doors.push_back(Door(x,y,2,0,false,-1));

                    current_door=vector_doors.size()-1;

                    generate_identifiers();
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select door for editing.
            if(keystates[SDLK_RETURN]){
                bool door_here=false;
                for(int j=0;j<vector_doors.size();j++){
                    if(collision_check(x,y,w,h,vector_doors.at(j).x,vector_doors.at(j).y,vector_doors.at(j).w,vector_doors.at(j).h)){
                        if(j!=current_door){
                            current_door=j;
                            door_here=true;
                        }
                    }
                }

                if(!door_here){
                    current_door=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing doors.
            if(keystates[SDLK_EQUALS]){
                //Check through the doors until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_doors.size();i++){
                    //If we have found an existing one.
                    if(i>current_door){
                        current_door=i;

                        //Snap the brush to this one's location.
                        x=vector_doors[i].x;
                        y=vector_doors[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing doors.
            if(keystates[SDLK_MINUS]){
                if(vector_doors.size()>0 && current_door==-1){
                    current_door=vector_doors.size()-1;
                }

                //Check through the vector until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_doors.size()-1;i>=0;i--){
                    //If we have found an existing one.
                    if(i<current_door){
                        current_door=i;

                        //Snap the brush to the one's location.
                        x=vector_doors[i].x;
                        y=vector_doors[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected door.
            if(current_door!=-1){
                //Move door.
                if(keystates[SDLK_m]){
                    bool door_here=false;
                    for(int j=0;j<vector_doors.size();j++){
                        if(collision_check(x,y,w,h,vector_doors[j].x,vector_doors[j].y,vector_doors[j].w,vector_doors[j].h)){
                            door_here=true;
                            break;
                        }
                    }
                    if(!door_here){
                        vector_doors[current_door].x=x;
                        vector_doors[current_door].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Toggle open.
                if(keystates[SDLK_o]){
                    vector_doors[current_door].open=!vector_doors[current_door].open;

                    keystates[SDLK_o]=NULL;
                }

                //Toggle type.
                if(keystates[SDLK_t]){
                    vector_doors[current_door].type++;
                    if(vector_doors[current_door].type>2){
                        vector_doors[current_door].type=0;
                    }

                    keystates[SDLK_t]=NULL;
                }

                //Toggle number.
                if(keystates[SDLK_n]){
                    vector_doors[current_door].number++;
                    if(vector_doors[current_door].type==1 && vector_doors[current_door].number>11){
                        vector_doors[current_door].number=0;
                    }
                    else if(vector_doors[current_door].type==0 && vector_doors[current_door].number>3){
                        vector_doors[current_door].number=0;
                    }
                    else if(vector_doors[current_door].type==2 && vector_doors[current_door].number>0){
                        vector_doors[current_door].number=0;
                    }

                    keystates[SDLK_n]=NULL;
                }
            }
        }

        //**************************//
        // Editing the traps layer: //
        //**************************//

        else if(layer_current==7){
            //Create trap.
            if(keystates[SDLK_SPACE] && trap_current!=0){
                bool trap_here=false;
                for(int j=0;j<vector_traps.size();j++){
                    if(collision_check(x,y,w,h,vector_traps[j].x,vector_traps[j].y,vector_traps[j].w,vector_traps[j].h)){
                        trap_here=true;
                        break;
                    }
                }
                if(!trap_here){
                    bool active=true;
                    //If the trap is a branch or stone slab.
                    if(trap_current==4 || trap_current==6){
                        active=false;
                    }
                    vector_traps.push_back(Trap(x,y,trap_current,active,-1));

                    current_trap=vector_traps.size()-1;

                    generate_identifiers();
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select trap for editing.
            if(keystates[SDLK_RETURN]){
                bool trap_here=false;
                for(int j=0;j<vector_traps.size();j++){
                    if(collision_check(x,y,w,h,vector_traps.at(j).x,vector_traps.at(j).y,vector_traps.at(j).w,vector_traps.at(j).h)){
                        if(j!=current_trap){
                            current_trap=j;
                            trap_here=true;
                        }
                    }
                }

                if(!trap_here){
                    current_trap=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing traps.
            if(keystates[SDLK_EQUALS]){
                //Check through the traps until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_traps.size();i++){
                    //If we have found an existing one.
                    if(i>current_trap){
                        //
                        current_trap=i;

                        //Snap the brush to this one's location.
                        x=vector_traps[i].x;
                        y=vector_traps[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing traps.
            if(keystates[SDLK_MINUS]){
                //
                if(vector_traps.size()>0 && current_trap==-1){
                    current_trap=vector_traps.size()-1;
                }

                //Check through the vector until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_traps.size()-1;i>=0;i--){
                    //If we have found an existing one.
                    if(i<current_trap){
                        //
                        current_trap=i;

                        //Snap the brush to the one's location.
                        x=vector_traps[i].x;
                        y=vector_traps[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected trap.
            if(current_trap!=-1){
                //Move trap.
                if(keystates[SDLK_m]){
                    bool trap_here=false;
                    for(int j=0;j<vector_traps.size();j++){
                        if(collision_check(x,y,w,h,vector_traps[j].x,vector_traps[j].y,vector_traps[j].w,vector_traps[j].h)){
                            trap_here=true;
                            break;
                        }
                    }
                    if(!trap_here){
                        vector_traps[current_trap].x=x;
                        vector_traps[current_trap].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Toggle active.
                if(keystates[SDLK_a]){
                    vector_traps[current_trap].active=!vector_traps[current_trap].active;

                    keystates[SDLK_a]=NULL;
                }

                //Toggle type.
                if(keystates[SDLK_t]){
                    vector_traps[current_trap].type++;
                    if(vector_traps[current_trap].type>8){
                        vector_traps[current_trap].type=1;
                    }

                    keystates[SDLK_t]=NULL;
                }
            }
        }

        //**************************//
        // Editing the signs layer: //
        //**************************//

        else if(layer_current==8){
            //Create sign.
            if(keystates[SDLK_SPACE]){
                bool sign_here=false;
                for(int j=0;j<vector_signs.size();j++){
                    if(collision_check(x,y,w,h,vector_signs[j].x,vector_signs[j].y,vector_signs[j].w,vector_signs[j].h)){
                        sign_here=true;
                        break;
                    }
                }
                if(!sign_here){
                    vector_signs.push_back(Sign(x,y,"",0,0));

                    current_sign=vector_signs.size()-1;
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select sign for editing.
            if(keystates[SDLK_RETURN]){
                bool sign_here=false;
                for(int j=0;j<vector_signs.size();j++){
                    if(collision_check(x,y,w,h,vector_signs.at(j).x,vector_signs.at(j).y,vector_signs.at(j).w,vector_signs.at(j).h)){
                        if(j!=current_sign){
                            current_sign=j;
                            sign_here=true;
                        }
                    }
                }

                if(!sign_here){
                    current_sign=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing signs.
            if(keystates[SDLK_EQUALS]){
                //Check through the signs until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_signs.size();i++){
                    //If we have found an existing one.
                    if(i>current_sign){
                        current_sign=i;

                        //Snap the brush to this one's location.
                        x=vector_signs[i].x;
                        y=vector_signs[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing signs.
            if(keystates[SDLK_MINUS]){
                if(vector_signs.size()>0 && current_sign==-1){
                    current_sign=vector_signs.size()-1;
                }

                //Check through the vector until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_signs.size()-1;i>=0;i--){
                    //If we have found an existing one.
                    if(i<current_sign){
                        current_sign=i;

                        //Snap the brush to the one's location.
                        x=vector_signs[i].x;
                        y=vector_signs[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected sign.
            if(current_sign!=-1){
                //Move sign.
                if(keystates[SDLK_m]){
                    bool sign_here=false;
                    for(int j=0;j<vector_signs.size();j++){
                        if(collision_check(x,y,w,h,vector_signs[j].x,vector_signs[j].y,vector_signs[j].w,vector_signs[j].h)){
                            sign_here=true;
                            break;
                        }
                    }
                    if(!sign_here){
                        vector_signs[current_sign].x=x;
                        vector_signs[current_sign].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Edit message.
                if(keystates[SDLK_e]){
                    edit_input=17;
                    edit_string="";
                    SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                    keystates[SDLK_e]=NULL;
                }

                //Toggle type.
                if(keystates[SDLK_t]){
                    vector_signs[current_sign].type++;
                    if(vector_signs[current_sign].type>1){
                        vector_signs[current_sign].type=0;
                    }

                    keystates[SDLK_t]=NULL;
                }

                //Toggle font type.
                if(keystates[SDLK_f]){
                    vector_signs[current_sign].font_type++;
                    if(vector_signs[current_sign].font_type>1){
                        vector_signs[current_sign].font_type=0;
                    }

                    keystates[SDLK_f]=NULL;
                }
            }
        }

        //****************************//
        // Editing the springs layer: //
        //****************************//

        else if(layer_current==9){
            //Create spring.
            if(keystates[SDLK_SPACE]){
                bool sign_here=false;
                for(int j=0;j<vector_springs.size();j++){
                    if(collision_check(x,y,SPRING_SIZE,SPRING_SIZE,vector_springs[j].x,vector_springs[j].y,SPRING_SIZE,SPRING_SIZE)){
                        sign_here=true;
                        break;
                    }
                }
                if(!sign_here){
                    vector_springs.push_back(Spring(x,y,20.0,UP));

                    current_spring=vector_springs.size()-1;
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select spring for editing.
            if(keystates[SDLK_RETURN]){
                bool sign_here=false;
                for(int j=0;j<vector_springs.size();j++){
                    if(collision_check(x,y,SPRING_SIZE,SPRING_SIZE,vector_springs.at(j).x,vector_springs.at(j).y,SPRING_SIZE,SPRING_SIZE)){
                        if(j!=current_spring){
                            current_spring=j;
                            sign_here=true;
                        }
                    }
                }

                if(!sign_here){
                    current_spring=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing springs.
            if(keystates[SDLK_EQUALS]){
                //Check through the springs until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_springs.size();i++){
                    //If we have found an existing one.
                    if(i>current_spring){
                        current_spring=i;

                        //Snap the brush to this one's location.
                        x=vector_springs[i].x;
                        y=vector_springs[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing springs.
            if(keystates[SDLK_MINUS]){
                if(vector_springs.size()>0 && current_spring==-1){
                    current_spring=vector_springs.size()-1;
                }

                //Check through the vector until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_springs.size()-1;i>=0;i--){
                    //If we have found an existing one.
                    if(i<current_spring){
                        current_spring=i;

                        //Snap the brush to the one's location.
                        x=vector_springs[i].x;
                        y=vector_springs[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected spring.
            if(current_spring!=-1){
                //Move spring.
                if(keystates[SDLK_m]){
                    bool sign_here=false;
                    for(int j=0;j<vector_springs.size();j++){
                        if(collision_check(x,y,SPRING_SIZE,SPRING_SIZE,vector_springs[j].x,vector_springs[j].y,SPRING_SIZE,SPRING_SIZE)){
                            sign_here=true;
                            break;
                        }
                    }
                    if(!sign_here){
                        vector_springs[current_spring].x=x;
                        vector_springs[current_spring].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Change direction.
                if(keystates[SDLK_d]){
                    vector_springs[current_spring].direction++;
                    if(vector_springs[current_spring].direction>LEFT_DOWN){
                        vector_springs[current_spring].direction=LEFT;
                    }

                    keystates[SDLK_d]=NULL;
                }

                //Edit speed.
                if(keystates[SDLK_s]){
                    edit_input=22;
                    edit_string="";
                    SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                    keystates[SDLK_s]=NULL;
                }
            }
        }

        //*****************************//
        // Editing the boosters layer: //
        //*****************************//

        else if(layer_current==10){
            //Create booster.
            if(keystates[SDLK_SPACE]){
                bool sign_here=false;
                for(int j=0;j<vector_boosters.size();j++){
                    if(collision_check(x,y,BOOSTER_SIZE,BOOSTER_SIZE,vector_boosters[j].x,vector_boosters[j].y,BOOSTER_SIZE,BOOSTER_SIZE)){
                        sign_here=true;
                        break;
                    }
                }
                if(!sign_here){
                    vector_boosters.push_back(Booster(x,y,30.0,RIGHT));

                    current_booster=vector_boosters.size()-1;
                }

                keystates[SDLK_SPACE]=NULL;
            }

            //Select spring for editing.
            if(keystates[SDLK_RETURN]){
                bool sign_here=false;
                for(int j=0;j<vector_boosters.size();j++){
                    if(collision_check(x,y,BOOSTER_SIZE,BOOSTER_SIZE,vector_boosters.at(j).x,vector_boosters.at(j).y,BOOSTER_SIZE,BOOSTER_SIZE)){
                        if(j!=current_booster){
                            current_booster=j;
                            sign_here=true;
                        }
                    }
                }

                if(!sign_here){
                    current_booster=-1;
                }

                keystates[SDLK_RETURN]=NULL;
            }

            //Scroll forward through existing springs.
            if(keystates[SDLK_EQUALS]){
                //Check through the springs until either the next existing one is found or the end of the vector is reached.
                for(int i=0;i<vector_boosters.size();i++){
                    //If we have found an existing one.
                    if(i>current_booster){
                        current_booster=i;

                        //Snap the brush to this one's location.
                        x=vector_boosters[i].x;
                        y=vector_boosters[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_EQUALS]=NULL;
            }

            //Scroll backward through existing springs.
            if(keystates[SDLK_MINUS]){
                if(vector_boosters.size()>0 && current_booster==-1){
                    current_booster=vector_boosters.size()-1;
                }

                //Check through the vector until either the previous existing one is found or the beginning of the vector is reached.
                for(int i=vector_boosters.size()-1;i>=0;i--){
                    //If we have found an existing one.
                    if(i<current_booster){
                        current_booster=i;

                        //Snap the brush to the one's location.
                        x=vector_boosters[i].x;
                        y=vector_boosters[i].y;

                        //Stop looping through.
                        break;
                    }
                }

                keystates[SDLK_MINUS]=NULL;
            }

            //As long as there is currently a selected booster.
            if(current_booster!=-1){
                //Move booster.
                if(keystates[SDLK_m]){
                    bool sign_here=false;
                    for(int j=0;j<vector_boosters.size();j++){
                        if(collision_check(x,y,BOOSTER_SIZE,BOOSTER_SIZE,vector_boosters[j].x,vector_boosters[j].y,BOOSTER_SIZE,BOOSTER_SIZE)){
                            sign_here=true;
                            break;
                        }
                    }
                    if(!sign_here){
                        vector_boosters[current_booster].x=x;
                        vector_boosters[current_booster].y=y;
                    }

                    keystates[SDLK_m]=NULL;
                }

                //Change direction.
                if(keystates[SDLK_d]){
                    if(vector_boosters[current_booster].direction==LEFT){
                        vector_boosters[current_booster].direction=RIGHT;
                    }
                    else if(vector_boosters[current_booster].direction==RIGHT){
                        vector_boosters[current_booster].direction=LEFT;
                    }

                    keystates[SDLK_d]=NULL;
                }

                //Edit speed.
                if(keystates[SDLK_s]){
                    edit_input=24;
                    edit_string="";
                    SDL_EnableUNICODE(SDL_ENABLE);/**Enable unicode*/

                    keystates[SDLK_s]=NULL;
                }
            }
        }

        //If the player hits the screenshot key, take a screenshot.
        if(keystates[SDLK_F5]){
            main_window.screenshot();

            //Once the screenshot key has been hit, the player must release it for it to function again.
            keystates[SDLK_F5]=NULL;
        }

        if(keystates[SDLK_F1]){
            edit_input=7;

            keystates[SDLK_F1]=NULL;
        }

        if(keystates[SDLK_F2]){
            edit_input=8;

            keystates[SDLK_F2]=NULL;
        }

        if(keystates[SDLK_F3]){
            edit_input=9;

            keystates[SDLK_F3]=NULL;
        }

        if(keystates[SDLK_F4]){
            edit_input=10;

            keystates[SDLK_F4]=NULL;
        }

        if(keystates[SDLK_h]){
            option_display_help=!option_display_help;

            keystates[SDLK_h]=NULL;
        }

        if(keystates[SDLK_ESCAPE] && option_display_help==true){
            option_display_help=false;

            keystates[SDLK_ESCAPE]=NULL;
        }

        if(keystates[SDLK_LEFTBRACKET]){
            option_display_overlays=!option_display_overlays;

            keystates[SDLK_LEFTBRACKET]=NULL;
        }

        if(keystates[SDLK_RIGHTBRACKET]){
            option_display_grid=!option_display_grid;

            keystates[SDLK_RIGHTBRACKET]=NULL;
        }

        if(keystates[SDLK_F10]){
            edit_input=6;
        }

        //Copy tile.
        if(keystates[SDLK_c]){
            reset_copy();
            for(short i=0;i<full_brush_w;i++){
                for(short n=0;n<full_brush_h;n++){
                    //Set the brush's current tile position.
                    int brush_current_x=(int)((int)(x+w*i)/w);
                    int brush_current_y=(int)((int)(y+h*n)/h);

                    if(layer_current==0){
                        copy_solidity[i][n]=level.tile_array[brush_current_x][brush_current_y].solidity;
                        copy_sprite[i][n]=level.tile_array[brush_current_x][brush_current_y].sprite;
                        copy_special[i][n]=level.tile_array[brush_current_x][brush_current_y].special;
                        copy_foreground[i][n]=level.tile_array[brush_current_x][brush_current_y].foreground;
                        copy_slope[i][n]=level.tile_array[brush_current_x][brush_current_y].angle;
                        /**for(short j=0;j<TILE_W;j++){
                            copy_height_mask[i][n][j]=level.tile_array[brush_current_x][brush_current_y].height_mask[j];
                        }*/
                    }
                    else if(layer_current==2){
                        copy_sprite[i][n]=level.tile_background_array[brush_current_x][brush_current_y].sprite;
                    }
                    else if(layer_current==1){
                        for(int j=0;j<vector_items.size();j++){
                            if(collision_check(x+w*i,y+h*n,w,h,vector_items.at(j).x,vector_items.at(j).y,vector_items.at(j).w,vector_items.at(j).h)){
                                copy_item_type[i][n]=vector_items.at(j).type;
                            }
                        }
                    }
                    else if(layer_current==3){
                        for(int j=0;j<vector_npcs.size();j++){
                            //
                            if(collision_check(x+w*i,y+h*n,w,h,vector_npcs.at(j).x,vector_npcs.at(j).y,vector_npcs.at(j).w,vector_npcs.at(j).h)){
                                copy_npc_type[i][n]=vector_npcs.at(j).type;
                            }
                        }
                    }
                }
            }

            keystates[SDLK_c]=NULL;
        }

        //Paste tile.
        /**if(keystates[SDLK_v]){
            for(short i=0;i<full_brush_w;i++){
                for(short n=0;n<full_brush_h;n++){
                    //Set the brush's current tile position.
                    int brush_current_x=(int)((int)(x+w*i)/w);
                    int brush_current_y=(int)((int)(y+h*n)/h);

                    if(layer_current==0){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_solidity[i][n]!=-1){
                            level.tile_array[brush_current_x][brush_current_y].solidity=copy_solidity[i][n];
                            level.tile_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                            level.tile_array[brush_current_x][brush_current_y].special=copy_special[i][n];
                            level.tile_array[brush_current_x][brush_current_y].foreground=copy_foreground[i][n];
                            level.tile_array[brush_current_x][brush_current_y].angle=copy_slope[i][n];
                            for(short j=0;j<TILE_W;j++){
                                level.tile_array[brush_current_x][brush_current_y].height_mask[j]=copy_height_mask[i][n][j];
                            }
                        }
                        else{
                            level.tile_array[brush_current_x][brush_current_y].solidity=0;
                            level.tile_array[brush_current_x][brush_current_y].sprite=0;
                            level.tile_array[brush_current_x][brush_current_y].special=0;
                            level.tile_array[brush_current_x][brush_current_y].foreground=false;
                            level.tile_array[brush_current_x][brush_current_y].angle=180;
                            for(short j=0;j<TILE_W;j++){
                                level.tile_array[brush_current_x][brush_current_y].height_mask[j]=TILE_H-1;
                            }
                        }
                    }
                    else if(layer_current==2){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_sprite[i][n]!=-1){
                            level.tile_background_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                        }
                        else{
                            level.tile_background_array[brush_current_x][brush_current_y].sprite=0;
                        }
                    }
                    else if(layer_current==1){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_item_type[i][n]!=-1){
                            create_item(i,n,copy_item_type[i][n]);
                        }
                    }
                    else if(layer_current==3){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_npc_type[i][n]!=-1){
                            create_npc(i,n,copy_npc_type[i][n]);
                        }
                    }
                }
            }

            //keystates[SDLK_v]=NULL;
        }

        //Erase tile.
        if(keystates[SDLK_DELETE]){
            for(short i=0;i<full_brush_w;i++){
                for(short n=0;n<full_brush_h;n++){
                    //Set the brush's current tile position.
                    int brush_current_x=(int)((int)(x+w*i)/w);
                    int brush_current_y=(int)((int)(y+h*n)/h);

                    if(layer_current==0){
                        level.tile_array[brush_current_x][brush_current_y].solidity=0;
                        level.tile_array[brush_current_x][brush_current_y].sprite=0;
                        level.tile_array[brush_current_x][brush_current_y].special=0;
                        level.tile_array[brush_current_x][brush_current_y].foreground=0;
                        level.tile_array[brush_current_x][brush_current_y].angle=180;
                        for(short j=0;j<TILE_W;j++){
                            level.tile_array[brush_current_x][brush_current_y].height_mask[j]=TILE_H-1;
                        }
                    }
                    else if(layer_current==2){
                        level.tile_background_array[brush_current_x][brush_current_y].sprite=0;
                    }
                    else if(layer_current==1){
                        for(int j=0;j<vector_items.size();j++){
                            if(collision_check(x+w*i,y+h*n,w,h,vector_items.at(j).x,vector_items.at(j).y,vector_items.at(j).w,vector_items.at(j).h)){
                                vector_items.erase(vector_items.begin()+j);
                            }
                        }
                    }
                    else if(layer_current==3){
                        for(int j=0;j<vector_npcs.size();j++){
                            if(collision_check(x+w*i,y+h*n,w,h,vector_npcs.at(j).x,vector_npcs.at(j).y,vector_npcs.at(j).w,vector_npcs.at(j).h)){
                                vector_npcs.erase(vector_npcs.begin()+j);
                            }
                        }
                    }
                }
            }

            if(layer_current==4 && current_platform!=-1){
                generate_identifiers(0,vector_moving_platforms[current_platform].identifier);
                vector_moving_platforms.erase(vector_moving_platforms.begin()+current_platform);
                current_platform=-1;
                generate_identifiers();
            }
            if(layer_current==5 && current_trigger!=-1){
                vector_triggers.erase(vector_triggers.begin()+current_trigger);
                current_trigger=-1;
            }
            if(layer_current==6 && current_door!=-1){
                generate_identifiers(1,vector_doors[current_door].identifier);
                vector_doors.erase(vector_doors.begin()+current_door);
                current_door=-1;
                generate_identifiers();
            }
            if(layer_current==7 && current_trap!=-1){
                generate_identifiers(2,vector_traps[current_trap].identifier);
                vector_traps.erase(vector_traps.begin()+current_trap);
                current_trap=-1;
                generate_identifiers();
            }
            if(layer_current==8 && current_sign!=-1){
                vector_signs.erase(vector_signs.begin()+current_sign);
                current_sign=-1;
            }

            //keystates[SDLK_DELETE]=NULL;
        }*/
    }

    //Get move_speed.
    if(edit_input==1){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_move_speed=0.0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_move_speed;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply move_speed change.
    if(edit_input==2){
        vector_moving_platforms[current_platform].move_speed=edit_move_speed;

        edit_input=0;
    }

    //Get repeat_time.
    if(edit_input==13){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_repeat_time=0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_repeat_time;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply repeat_time change.
    if(edit_input==14){
        vector_triggers[current_trigger].repeat_time=edit_repeat_time;

        edit_input=0;
    }

    //Get slope.
    if(edit_input==11){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_sprite=0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_sprite;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply slope change.
    if(edit_input==12){
        for(short i=0;i<full_brush_w;i++){
            for(short n=0;n<full_brush_h;n++){
                //Set the brush's current tile position.
                int brush_current_x=(int)((int)(x+w*i)/w);
                int brush_current_y=(int)((int)(y+h*n)/h);

                level.tile_array[brush_current_x][brush_current_y].angle=edit_sprite;
            }
        }

        edit_input=0;
    }

    //Get height mask.
    if(edit_input==15){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_sprite=0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_sprite;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply height mask change.
    if(edit_input==16){
        for(short i=0;i<full_brush_w;i++){
            for(short n=0;n<full_brush_h;n++){
                //Set the brush's current tile position.
                int brush_current_x=(int)((int)(x+w*i)/w);
                int brush_current_y=(int)((int)(y+h*n)/h);

                //Don't attempt to assign a non-existent height mask.
                if(edit_sprite>number_of_height_masks){
                    edit_sprite=0;
                }

                /**for(short j=0;j<TILE_W;j++){
                    level.tile_array[brush_current_x][brush_current_y].height_mask[j]=height_masks[edit_sprite][j];
                }*/
            }
        }

        edit_input=0;
    }

    //Get sign message.
    if(edit_input==17){
        ///if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            ///edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            ///keystates[SDLK_BACKSPACE]=NULL;
        ///}
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && (keystates[SDLK_RCTRL] || keystates[SDLK_LCTRL])){
            edit_string+="\xA";

            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }
        else if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_input++;

            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }
        else if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply sign message change.
    if(edit_input==18){
        if(current_sign!=-1){
            vector_signs[current_sign].message=edit_string;
        }

        edit_input=0;
    }

    //Get sprite.
    if(edit_input==4){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_sprite=0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_sprite;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply sprite change.
    if(edit_input==5){
        for(short i=0;i<full_brush_w;i++){
            for(short n=0;n<full_brush_h;n++){
                //Set the brush's current tile position.
                int brush_current_x=(int)((int)(x+w*i)/w);
                int brush_current_y=(int)((int)(y+h*n)/h);

                //Terrain tiles.
                if(layer_current==0){
                    level.tile_array[brush_current_x][brush_current_y].sprite=edit_sprite;
                }
                //Background tiles.
                else if(layer_current==2){
                    level.tile_background_array[brush_current_x][brush_current_y].sprite=edit_sprite;
                }
            }
        }

        edit_input=0;
    }

    //Get goal_level_to_load.
    if(edit_input==19){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_goal_level_to_load=0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_goal_level_to_load;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply goal_level_to_load change.
    if(edit_input==20){
        vector_items[item_selected].goal_level_to_load=edit_goal_level_to_load;

        edit_input=0;
    }

    //Edit spring speed.
    if(edit_input==22){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_move_speed=0.0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_move_speed;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply spring speed change.
    if(edit_input==23){
        vector_springs[current_spring].speed=edit_move_speed;

        edit_input=0;
    }

    //Edit booster speed.
    if(edit_input==24){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if((keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            edit_move_speed=0.0;
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_move_speed;

            edit_input++;

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Apply booster speed change.
    if(edit_input==25){
        vector_boosters[current_booster].speed=edit_move_speed;

        edit_input=0;
    }

    //Confirm quit.
    if(edit_input==6){
        if(keystates[SDLK_y]){
            quit_game();
        }

        if(keystates[SDLK_n] || keystates[SDLK_ESCAPE]){
            edit_input=0;

            keystates[SDLK_n]=NULL;
        }
    }

    //Confirm new.
    if(edit_input==7){
        if(keystates[SDLK_y]){
            edit_new_x=0;
            edit_new_y=0;
            edit_string="";
            //Enable unicode.
            SDL_EnableUNICODE(SDL_ENABLE);
            edit_input=3;
            new_level_is_random=false;
        }

        if(keystates[SDLK_n] || keystates[SDLK_ESCAPE]){
            edit_input=0;

            keystates[SDLK_n]=NULL;
        }
    }

    //Get new level dimensions.
    if(edit_input==3){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if(edit_new_x==0 && (keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_new_x;

            edit_string="";

            keystates[SDLK_RETURN]=NULL;
        }
        else if(edit_new_x!=0 && (keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_new_y;

            edit_input=0;

            if(!new_level_is_random){
                level.new_level(edit_new_x,edit_new_y);
            }
            else{
                level.new_random_level(edit_new_x,edit_new_y);
            }

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Confirm resize.
    if(edit_input==8){
        if(keystates[SDLK_y]){
            edit_new_x=0;
            edit_new_y=0;
            edit_string="";
            //Enable unicode.
            SDL_EnableUNICODE(SDL_ENABLE);
            edit_input=21;
        }

        if(keystates[SDLK_n] || keystates[SDLK_ESCAPE]){
            edit_input=0;

            keystates[SDLK_n]=NULL;
        }
    }

    //Get level resize dimensions.
    if(edit_input==21){
        if(keystates[SDLK_BACKSPACE] && edit_string.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            edit_string.erase(edit_string.length()-1);/**Remove one character from the end of the string.*/
            keystates[SDLK_BACKSPACE]=NULL;
        }
        if(edit_new_x==0 && (keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_new_x;

            edit_string="";

            keystates[SDLK_RETURN]=NULL;
        }
        else if(edit_new_x!=0 && (keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]) && edit_string.length()!=0){
            ss.clear();ss.str("");ss<<edit_string;ss>>edit_new_y;

            edit_input=0;

            level.resize_level(edit_new_x,edit_new_y);

            keystates[SDLK_RETURN]=NULL;
        }
        if(keystates[SDLK_ESCAPE]){
            edit_input=0;
            keystates[SDLK_ESCAPE]=NULL;
        }
    }

    //Confirm save.
    if(edit_input==9){
        if(keystates[SDLK_y]){
            level.save_level(false);
            edit_input=0;
        }

        if(keystates[SDLK_n] || keystates[SDLK_ESCAPE]){
            edit_input=0;

            keystates[SDLK_n]=NULL;
        }
    }

    //Confirm new random.
    if(edit_input==10){
        if(keystates[SDLK_y]){
            edit_new_x=0;
            edit_new_y=0;
            edit_string="";
            //Enable unicode.
            SDL_EnableUNICODE(SDL_ENABLE);
            edit_input=3;
            new_level_is_random=true;
        }

        if(keystates[SDLK_n] || keystates[SDLK_ESCAPE]){
            edit_input=0;

            keystates[SDLK_n]=NULL;
        }
    }

    //Clicking the left mouse button allows the windows to be dragged.
    if(edit_input==4 || option_display_help==true){
        int mouse_x=0,mouse_y=0;

        SDL_GetMouseState(&mouse_x,&mouse_y);

        if(window_big_moving==true){
            window_big_x=mouse_x;
            window_big_y=mouse_y;
        }

        if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
            if(collision_check(mouse_x,mouse_y,2,2,window_big_x,window_big_y,25,25)){
                window_big_moving=true;
            }
        }
        else{
            window_big_moving=false;
        }
    }
}

void Brush::move(){
    int move_chunk=0;
    int speed_modifier=1;

    if(speed_mode){
        speed_modifier=4;
    }

    if(layer_current==0 || layer_current==2 || layer_current==5 || layer_current==6 || layer_current==8 || layer_current==9 || layer_current==10){
        move_chunk=32;
    }
    else if(layer_current==1 || layer_current==3 || layer_current==4 || layer_current==7){
        move_chunk=16;
    }

    int total=fabs(move_chunk)*speed_modifier;

    if(desert_addition){
        total=move_chunk*30;
        move_state=RIGHT;
    }

    for(int i=total;i>0;){
        //If we have run_chunk or more pixels left to move,
        //we will move run_chunk pixels, call handle_events(), and loop back up here.

        //Or, if we have less than run_chunk pixels left to move,
        //we will move whatever pixels we have left and call handle_events() once more.
        if(i<move_chunk){
            move_chunk=i;
            i=0;
        }

        //Move.
        if(move_state==LEFT){
            x-=move_chunk;
        }
        else if(move_state==RIGHT){
            x+=move_chunk;
        }
        else if(move_state==UP){
            y-=move_chunk;
        }
        else if(move_state==DOWN){
            y+=move_chunk;
        }
        else if(move_state==LEFT_UP){
            x-=move_chunk;
            y-=move_chunk;
        }
        else if(move_state==LEFT_DOWN){
            x-=move_chunk;
            y+=move_chunk;
        }
        else if(move_state==RIGHT_UP){
            x+=move_chunk;
            y-=move_chunk;
        }
        else if(move_state==RIGHT_DOWN){
            x+=move_chunk;
            y+=move_chunk;
        }

        //If we still have pixels left to move.
        if(i!=0){
            i-=move_chunk;
        }

        handle_events();
    }

    if(desert_addition && edit_input==0){
        for(short i=0;i<full_brush_w;i++){
            for(short n=0;n<full_brush_h;n++){
                //Set the brush's current tile position.
                int brush_current_x=(int)((int)(x+w*i)/w);
                int brush_current_y=(int)((int)(y+h*n)/h);

                if(layer_current==0){
                    //The copy variables start at -1, so only paste if something has been copied.
                    if(copy_solidity[i][n]!=-1){
                        level.tile_array[brush_current_x][brush_current_y].solidity=copy_solidity[i][n];
                        level.tile_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                        level.tile_array[brush_current_x][brush_current_y].special=copy_special[i][n];
                        level.tile_array[brush_current_x][brush_current_y].foreground=copy_foreground[i][n];
                        level.tile_array[brush_current_x][brush_current_y].angle=copy_slope[i][n];
                        /**for(short j=0;j<TILE_W;j++){
                            level.tile_array[brush_current_x][brush_current_y].height_mask[j]=copy_height_mask[i][n][j];
                        }*/
                    }
                    else{
                        level.tile_array[brush_current_x][brush_current_y].solidity=0;
                        level.tile_array[brush_current_x][brush_current_y].sprite=0;
                        level.tile_array[brush_current_x][brush_current_y].special=0;
                        level.tile_array[brush_current_x][brush_current_y].foreground=false;
                        level.tile_array[brush_current_x][brush_current_y].angle=180;
                        /**for(short j=0;j<TILE_W;j++){
                            level.tile_array[brush_current_x][brush_current_y].height_mask[j]=TILE_H-1;
                        }*/
                    }
                }
                else if(layer_current==2){
                    //The copy variables start at -1, so only paste if something has been copied.
                    if(copy_sprite[i][n]!=-1){
                        level.tile_background_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                    }
                    else{
                        level.tile_background_array[brush_current_x][brush_current_y].sprite=0;
                    }
                }
                else if(layer_current==1){
                    //The copy variables start at -1, so only paste if something has been copied.
                    if(copy_item_type[i][n]!=-1){
                        create_item(i,n,copy_item_type[i][n]);
                    }
                }
                else if(layer_current==3){
                    //The copy variables start at -1, so only paste if something has been copied.
                    if(copy_npc_type[i][n]!=-1){
                        create_npc(i,n,copy_npc_type[i][n]);
                    }
                }
            }
        }
    }
}

void Brush::handle_events(){
    //Keep the player in the level map's bounds.
    if(x<0){
        x=0;
    }
    if(x+w*full_brush_w>level.level_x){
        x=level.level_x-w*full_brush_w;
    }
    if(y<0){
        y=0;
    }
    if(y+h*full_brush_h>level.level_y){
        y=level.level_y-h*full_brush_h;
    }

    if(edit_input==0){
        //Paste tile.
        if(keystates[SDLK_v]){
            for(short i=0;i<full_brush_w;i++){
                for(short n=0;n<full_brush_h;n++){
                    //Set the brush's current tile position.
                    int brush_current_x=(int)((int)(x+w*i)/w);
                    int brush_current_y=(int)((int)(y+h*n)/h);

                    if(layer_current==0){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_solidity[i][n]!=-1){
                            level.tile_array[brush_current_x][brush_current_y].solidity=copy_solidity[i][n];
                            level.tile_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                            level.tile_array[brush_current_x][brush_current_y].special=copy_special[i][n];
                            level.tile_array[brush_current_x][brush_current_y].foreground=copy_foreground[i][n];
                            level.tile_array[brush_current_x][brush_current_y].angle=copy_slope[i][n];
                            /**for(short j=0;j<TILE_W;j++){
                                level.tile_array[brush_current_x][brush_current_y].height_mask[j]=copy_height_mask[i][n][j];
                            }*/
                        }
                        else{
                            level.tile_array[brush_current_x][brush_current_y].solidity=0;
                            level.tile_array[brush_current_x][brush_current_y].sprite=0;
                            level.tile_array[brush_current_x][brush_current_y].special=0;
                            level.tile_array[brush_current_x][brush_current_y].foreground=false;
                            level.tile_array[brush_current_x][brush_current_y].angle=180;
                            /**for(short j=0;j<TILE_W;j++){
                                level.tile_array[brush_current_x][brush_current_y].height_mask[j]=TILE_H-1;
                            }*/
                        }
                    }
                    else if(layer_current==2){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_sprite[i][n]!=-1){
                            level.tile_background_array[brush_current_x][brush_current_y].sprite=copy_sprite[i][n];
                        }
                        else{
                            level.tile_background_array[brush_current_x][brush_current_y].sprite=0;
                        }
                    }
                    else if(layer_current==1){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_item_type[i][n]!=-1){
                            create_item(i,n,copy_item_type[i][n]);
                        }
                    }
                    else if(layer_current==3){
                        //The copy variables start at -1, so only paste if something has been copied.
                        if(copy_npc_type[i][n]!=-1){
                            create_npc(i,n,copy_npc_type[i][n]);
                        }
                    }
                }
            }

            //keystates[SDLK_v]=NULL;
        }

        //Erase tile.
        if(keystates[SDLK_DELETE]){
            for(short i=0;i<full_brush_w;i++){
                for(short n=0;n<full_brush_h;n++){
                    //Set the brush's current tile position.
                    int brush_current_x=(int)((int)(x+w*i)/w);
                    int brush_current_y=(int)((int)(y+h*n)/h);

                    if(layer_current==0){
                        level.tile_array[brush_current_x][brush_current_y].solidity=0;
                        level.tile_array[brush_current_x][brush_current_y].sprite=0;
                        level.tile_array[brush_current_x][brush_current_y].special=0;
                        level.tile_array[brush_current_x][brush_current_y].foreground=0;
                        level.tile_array[brush_current_x][brush_current_y].angle=180;
                        /**for(short j=0;j<TILE_W;j++){
                            level.tile_array[brush_current_x][brush_current_y].height_mask[j]=TILE_H-1;
                        }*/
                    }
                    else if(layer_current==2){
                        level.tile_background_array[brush_current_x][brush_current_y].sprite=0;
                    }
                    else if(layer_current==1){
                        for(int j=0;j<vector_items.size();j++){
                            if(collision_check(x+w*i,y+h*n,w,h,vector_items.at(j).x,vector_items.at(j).y,vector_items.at(j).w,vector_items.at(j).h)){
                                vector_items.erase(vector_items.begin()+j);
                            }
                        }
                    }
                    else if(layer_current==3){
                        for(int j=0;j<vector_npcs.size();j++){
                            if(collision_check(x+w*i,y+h*n,w,h,vector_npcs.at(j).x,vector_npcs.at(j).y,vector_npcs.at(j).w,vector_npcs.at(j).h)){
                                vector_npcs.erase(vector_npcs.begin()+j);
                            }
                        }
                    }
                }
            }

            if(layer_current==4 && current_platform!=-1){
                generate_identifiers(0,vector_moving_platforms[current_platform].identifier);
                vector_moving_platforms.erase(vector_moving_platforms.begin()+current_platform);
                current_platform=-1;
                generate_identifiers();
            }
            if(layer_current==5 && current_trigger!=-1){
                vector_triggers.erase(vector_triggers.begin()+current_trigger);
                current_trigger=-1;
            }
            if(layer_current==6 && current_door!=-1){
                generate_identifiers(1,vector_doors[current_door].identifier);
                vector_doors.erase(vector_doors.begin()+current_door);
                current_door=-1;
                generate_identifiers();
            }
            if(layer_current==7 && current_trap!=-1){
                generate_identifiers(2,vector_traps[current_trap].identifier);
                vector_traps.erase(vector_traps.begin()+current_trap);
                current_trap=-1;
                generate_identifiers();
            }
            if(layer_current==8 && current_sign!=-1){
                vector_signs.erase(vector_signs.begin()+current_sign);
                current_sign=-1;
            }
            if(layer_current==9 && current_spring!=-1){
                vector_springs.erase(vector_springs.begin()+current_spring);
                current_spring=-1;
            }

            //keystates[SDLK_DELETE]=NULL;
        }
    }
}

void Brush::set_camera(){
    //Now center the camera on the player.
    camera.x=(x+w*full_brush_w/2)-(camera.w/2);
    camera.y=(y+h*full_brush_h/2)-(camera.h/2);
}

void Brush::render_background(){
    render_texture(0,0,camera.w,camera.h,background_static);
}

void Brush::render(){
    for(short i=0;i<full_brush_w;i++){
        for(short n=0;n<full_brush_h;n++){
            render_sprite((int)(x+w*i-camera.x),(int)(y+h*n-camera.y),56,32,sprite_sheet_brush,&sprites_box[layer_current]);
        }
    }
}
