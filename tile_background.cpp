/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tile_background.h"
#include "render.h"
#include "world.h"

using namespace std;

Tile_Background::Tile_Background(){
    //Set the tile's variables:

    sprite=0;

    x=0;
    y=0;
}

void Tile_Background::render(){
    //If the background tiles layer is currently being displayed.
    if(brush.layer_tiles_background){
        //If sprite is not zero (0 means no sprite), and the tile is in camera bounds, render the tile.
        if(sprite!=0 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),256,288,tiles_background,&brush.sprites_tiles_background[sprite]);
        }
    }
}
