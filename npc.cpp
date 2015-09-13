/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "npc.h"
#include "render.h"
#include "world.h"
#include "npc_dimensions.h"

using namespace std;

Npc::Npc(double get_x,double get_y,short get_type){
    type=get_type;

    x=get_x;
    y=get_y;

    w=NPC_SIZE;
    h=NPC_SIZE;

    sprites_npcs[1].x=0;
    sprites_npcs[1].y=0;
    sprites_npcs[1].w=32;
    sprites_npcs[1].h=29;

    sprites_npcs[2].x=32;
    sprites_npcs[2].y=0;
    sprites_npcs[2].w=81;
    sprites_npcs[2].h=44;

    sprites_npcs[3].x=113;
    sprites_npcs[3].y=0;
    sprites_npcs[3].w=27;
    sprites_npcs[3].h=27;

    sprites_npcs[4].x=140;
    sprites_npcs[4].y=0;
    sprites_npcs[4].w=32;
    sprites_npcs[4].h=17;

    sprites_npcs[5].x=172;
    sprites_npcs[5].y=0;
    sprites_npcs[5].w=28;
    sprites_npcs[5].h=15;

    sprites_npcs[6].x=200;
    sprites_npcs[6].y=0;
    sprites_npcs[6].w=26;
    sprites_npcs[6].h=51;

    sprites_npcs[7].x=226;
    sprites_npcs[7].y=0;
    sprites_npcs[7].w=24;
    sprites_npcs[7].h=24;

    sprites_npcs[8].x=258;
    sprites_npcs[8].y=0;
    sprites_npcs[8].w=32;
    sprites_npcs[8].h=64;

    sprites_npcs[9].x=290;
    sprites_npcs[9].y=0;
    sprites_npcs[9].w=32;
    sprites_npcs[9].h=32;

    sprites_npcs[10].x=322;
    sprites_npcs[10].y=0;
    sprites_npcs[10].w=31;
    sprites_npcs[10].h=64;

    sprites_npcs[11].x=353;
    sprites_npcs[11].y=0;
    sprites_npcs[11].w=21;
    sprites_npcs[11].h=32;

    sprites_npcs[12].x=373;
    sprites_npcs[12].y=0;
    sprites_npcs[12].w=32;
    sprites_npcs[12].h=78;

    sprites_npcs[13].x=405;
    sprites_npcs[13].y=0;
    sprites_npcs[13].w=32;
    sprites_npcs[13].h=78;

    sprites_npcs[14].x=437;
    sprites_npcs[14].y=0;
    sprites_npcs[14].w=16;
    sprites_npcs[14].h=32;

    sprites_npcs[15].x=453;
    sprites_npcs[15].y=0;
    sprites_npcs[15].w=59;
    sprites_npcs[15].h=77;

    sprites_npcs[16].x=512;
    sprites_npcs[16].y=0;
    sprites_npcs[16].w=43;
    sprites_npcs[16].h=35;

    sprites_npcs[17].x=555;
    sprites_npcs[17].y=0;
    sprites_npcs[17].w=44;
    sprites_npcs[17].h=79;

    sprites_npcs[18].x=599;
    sprites_npcs[18].y=0;
    sprites_npcs[18].w=44;
    sprites_npcs[18].h=79;

    sprites_npcs[19].x=640;
    sprites_npcs[19].y=0;
    sprites_npcs[19].w=32;
    sprites_npcs[19].h=24;

    sprites_npcs[20].x=672;
    sprites_npcs[20].y=0;
    sprites_npcs[20].w=32;
    sprites_npcs[20].h=16;

    sprites_npcs[21].x=704;
    sprites_npcs[21].y=0;
    sprites_npcs[21].w=32;
    sprites_npcs[21].h=28;

    sprites_npcs[22].x=736;
    sprites_npcs[22].y=0;
    sprites_npcs[22].w=32;
    sprites_npcs[22].h=16;

    sprites_npcs[23].x=0;
    sprites_npcs[23].y=79;
    sprites_npcs[23].w=74;
    sprites_npcs[23].h=48;

    sprites_npcs[24].x=74;
    sprites_npcs[24].y=79;
    sprites_npcs[24].w=53;
    sprites_npcs[24].h=35;

    sprites_npcs[25].x=127;
    sprites_npcs[25].y=79;
    sprites_npcs[25].w=25;
    sprites_npcs[25].h=52;

    sprites_npcs[26].x=152;
    sprites_npcs[26].y=79;
    sprites_npcs[26].w=63;
    sprites_npcs[26].h=29;

    sprites_npcs[27].x=215;
    sprites_npcs[27].y=79;
    sprites_npcs[27].w=34;
    sprites_npcs[27].h=46;

    sprites_npcs[28].x=249;
    sprites_npcs[28].y=79;
    sprites_npcs[28].w=32;
    sprites_npcs[28].h=32;

    sprites_npcs[29].x=249;
    sprites_npcs[29].y=79;
    sprites_npcs[29].w=32;
    sprites_npcs[29].h=32;

    sprites_npcs[30].x=257;
    sprites_npcs[30].y=79;
    sprites_npcs[30].w=32;
    sprites_npcs[30].h=32;

    sprites_npcs[31].x=257;
    sprites_npcs[31].y=79;
    sprites_npcs[31].w=32;
    sprites_npcs[31].h=32;

    sprites_npcs[32].x=289;
    sprites_npcs[32].y=79;
    sprites_npcs[32].w=64;
    sprites_npcs[32].h=64;

    sprites_npcs[33].x=353;
    sprites_npcs[33].y=79;
    sprites_npcs[33].w=40;
    sprites_npcs[33].h=20;

    sprites_npcs[34].x=393;
    sprites_npcs[34].y=79;
    sprites_npcs[34].w=100;
    sprites_npcs[34].h=66;

    sprites_npcs[35].x=493;
    sprites_npcs[35].y=79;
    sprites_npcs[35].w=100;
    sprites_npcs[35].h=66;

    sprites_npcs[36].x=586;
    sprites_npcs[36].y=79;
    sprites_npcs[36].w=64;
    sprites_npcs[36].h=32;

    sprites_npcs[37].x=650;
    sprites_npcs[37].y=79;
    sprites_npcs[37].w=29;
    sprites_npcs[37].h=52;

    sprites_npcs[38].x=679;
    sprites_npcs[38].y=79;
    sprites_npcs[38].w=77;
    sprites_npcs[38].h=43;

    sprites_npcs[39].x=0;
    sprites_npcs[39].y=145;
    sprites_npcs[39].w=26;
    sprites_npcs[39].h=46;

    sprites_npcs[40].x=32;
    sprites_npcs[40].y=0;
    sprites_npcs[40].w=81;
    sprites_npcs[40].h=44;

    sprites_npcs[41].x=26;
    sprites_npcs[41].y=145;
    sprites_npcs[41].w=64;
    sprites_npcs[41].h=64;

    sprites_npcs[42].x=90;
    sprites_npcs[42].y=145;
    sprites_npcs[42].w=54;
    sprites_npcs[42].h=17;

    sprites_npcs[43].x=144;
    sprites_npcs[43].y=145;
    sprites_npcs[43].w=43;
    sprites_npcs[43].h=42;

    sprites_npcs[44].x=187;
    sprites_npcs[44].y=145;
    sprites_npcs[44].w=77;
    sprites_npcs[44].h=43;

    sprites_npcs[45].x=264;
    sprites_npcs[45].y=145;
    sprites_npcs[45].w=77;
    sprites_npcs[45].h=43;

    sprites_npcs[46].x=341;
    sprites_npcs[46].y=145;
    sprites_npcs[46].w=77;
    sprites_npcs[46].h=43;

    sprites_npcs[47].x=418;
    sprites_npcs[47].y=145;
    sprites_npcs[47].w=77;
    sprites_npcs[47].h=43;

    sprites_npcs[48].x=495;
    sprites_npcs[48].y=145;
    sprites_npcs[48].w=77;
    sprites_npcs[48].h=43;
}

void Npc::render(){
    //If the npcs layer is currently being displayed.
    if(brush.layer_npcs){
        //If the npc is in camera bounds, render the npc.
        if(type!=0 && x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),768,384,sprite_sheet_npcs,&sprites_npcs[type]);
            render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),NPC_SIZE,NPC_SIZE,0.5,1.0,0.0,0.0);
            render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_npcs[type].w,sprites_npcs[type].h,0.25,1.0,0.0,0.0);
        }
    }
}
