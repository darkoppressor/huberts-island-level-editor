/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tile.h"
#include "render.h"
#include "world.h"

using namespace std;

Tile::Tile(){
    //Set the tile's variables:

    x=0;
    y=0;
    solidity=0;
    sprite=0;
    special=0;
    foreground=0;
    angle=180;
    /**for(short i=0;i<TILE_W;i++){
        height_mask[i]=TILE_H-1;
    }*/
}

void Tile::render(){
    //If the tiles layer is currently being displayed.
    if(brush.layer_tiles){
        //If sprite is not zero (0 means no sprite), and the tile is in camera bounds, render the tile.
        if(sprite!=0 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            if(!brush.WORLD_MAP){
                if(sprite==1 || sprite==2){
                    render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),512,32,water_tiles,&brush.sprites_tiles[sprite],0.50);
                }
                else{
                    render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),608,288,tiles,&brush.sprites_tiles[sprite]);
                }
            }
            else if(brush.WORLD_MAP){
                if(sprite==1 || sprite==2){
                    render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),512,32,water_tiles,&brush.worldmap_sprites_tiles[sprite],0.50);
                }
                else{
                    render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),608,288,worldmap_tiles,&brush.worldmap_sprites_tiles[sprite]);
                }
            }
        }

        //Overlays:

        //Solid.
        if(brush.option_display_overlays && solidity==1 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[1]);
        }

        //Cloud.
        if(brush.option_display_overlays && solidity==2 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[2]);
        }

        //Climbable.
        if(brush.option_display_overlays && special==1 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[3]);
        }

        //Climbable Top.
        if(brush.option_display_overlays && special==2 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[4]);
        }

        //Hazard.
        if(brush.option_display_overlays && special==3 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[5]);
        }

        //Water.
        if(brush.option_display_overlays && special==4 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[6]);
        }

        //Mirror.
        if(brush.option_display_overlays && special==5 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[0]);
        }

        //Item Holder.
        if(brush.option_display_overlays && special==6 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[7]);
        }

        if(brush.option_display_overlays && foreground && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,320,overlays,&brush.sprites_overlays[8]);
        }

        if(brush.option_display_overlays && angle!=180 && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            ss.clear();ss.str("");ss<<angle;ss<<"\xA7";msg=ss.str();
            font.show((int)(x-brush.camera.x),(int)(y-brush.camera.y),msg);
        }

        if(brush.option_display_grid && x>=brush.camera.x-TILE_W && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-TILE_H && y<=brush.camera.y+brush.camera.h){
            if(brush.layer_current==0 || brush.layer_current==2 || brush.layer_current==5 || brush.layer_current==6 || brush.layer_current==8 || brush.layer_current==9 || brush.layer_current==10){
                render_texture((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,32,grid);
            }
            else if(brush.layer_current==1 || brush.layer_current==3 || brush.layer_current==4 || brush.layer_current==7){
                render_texture((int)(x-brush.camera.x),(int)(y-brush.camera.y),32,32,grid_items);
            }
        }
    }
}
