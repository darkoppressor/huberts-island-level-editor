/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "height_masks.h"
#include "pixels.h"
#include "tile_sprites.h"

#include <SDL_image.h>

using namespace std;

vector<short> load_height_mask(std::string number){
    //Set the filename.
    string filename="data/height_masks/";
    filename+=number;
    filename+=".png";

    //Temporary storage for the image being checked.
    SDL_Surface* surface=NULL;

    //Load the image.
    surface=IMG_Load(filename.c_str());

    Uint8 check_red;
    Uint8 check_green;
    Uint8 check_blue;
    Uint8 check_alpha;

    vector<short> height_mask_loading;
    height_mask_loading.resize(TILE_W);

    //If the surface must be locked.
    if(SDL_MUSTLOCK(surface)){
        //Lock the surface.
        SDL_LockSurface(surface);
    }

    //Look through each column in turn.
    for(short x=0;x<TILE_W;x++){
        //Check each row for this column.
        for(short y=0;y<TILE_H;y++){
            //Retrieve this pixel.
            SDL_GetRGBA(surface_get_pixel(surface,x,y),surface->format,&check_red,&check_green,&check_blue,&check_alpha);

            //If the pixel is black, we have reached the height mark for this column.
            if(check_red==0 && check_green==0 && check_blue==0){
                //
                height_mask_loading[x]=TILE_H-1-y;

                //We're done with this column now.
                break;
            }
        }
    }

    //If the surface was locked.
    if(SDL_MUSTLOCK(surface)){
        //Unlock surface.
        SDL_UnlockSurface(surface);
    }

    //Free the surface.
    SDL_FreeSurface(surface);

    return height_mask_loading;
}
