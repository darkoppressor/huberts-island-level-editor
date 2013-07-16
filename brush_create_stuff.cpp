/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "brush.h"
#include "world.h"
#include "collision.h"

using namespace std;

void Brush::create_item(short i,short n,short type){
    bool item_here=false;
    for(int j=0;j<vector_items.size();j++){
        if(collision_check(x+w*i,y+h*n,w,h,vector_items[j].x,vector_items[j].y,vector_items[j].w,vector_items[j].h)){
            item_here=true;
            vector_items[j].type=type;
            break;
        }
    }
    if(!item_here){
        vector_items.push_back(Item(x+w*i,y+h*n,type,0,false));
    }
}

void Brush::create_npc(short i,short n,short type){
    bool npc_here=false;
    for(int j=0;j<vector_npcs.size();j++){
        if(collision_check(x+w*i,y+h*n,w,h,vector_npcs[j].x,vector_npcs[j].y,vector_npcs[j].w,vector_npcs[j].h)){
            npc_here=true;
            vector_npcs[j].type=type;
            break;
        }
    }
    if(!npc_here){
        vector_npcs.push_back(Npc(x+w*i,y+h*n,type));
    }
}
