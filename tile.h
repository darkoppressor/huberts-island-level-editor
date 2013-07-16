/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef tile_h
#define tile_h

#include "tile_sprites.h"

#include <SDL.h>

class Tile{
    private:
    public:
    //The constructor takes the tile's coordinates, as well as its various properties.
    Tile();

    //Render the tile to the screen.
    void render();

    //The coordinates of the tile. These are used for collision detection.
    double x,y;

    //Here we have the variables for storing the tile's properties. By mixing these different properties, all sorts of cool things are possible.
    //For instance, you could make an invisible but solid tile with tile properties '1 0 0'.

    //solidity governs whether the tile is completely passable, solid, or solid only on top.
    //0 = Passable.
    //1 = Solid.
    //2 = Solid only on top. (The player can jump through the bottom of this tile.)
    short solidity;

    //sprite governs the look of the tile. This corresponds to the tileset of the current level.
    short sprite;

    short special;

    bool foreground;

    short angle;

    ///short height_mask[TILE_W];
};

#endif
