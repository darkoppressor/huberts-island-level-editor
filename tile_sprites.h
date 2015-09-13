/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef tile_sprites_h
#define tile_sprites_h

//These two variables let the editor know how many tile sprites there are total, and how many are clutter tiles.

//This value should actually be the number of tiles + 1 (for the air tile) + 2 (for the water tiles).
const short TILE_SPRITES=173;

const short CLUTTER_TILES=5;

const short WORLDMAP_TILES=174;

//This value should actually be the number of tiles + 1.
const short BACKGROUND_TILES=73;

const int TILE_W=32;
const int TILE_H=32;

#endif
