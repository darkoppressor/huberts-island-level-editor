/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef level_h
#define level_h

#include "tile.h"
#include "tile_background.h"
#include "background_layer.h"

#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

struct Room{
    int x;
    int y;
    int room_size;
};

class Level{
    private:
    //Background music will be loaded into this variable.
    Mix_Music *music;

    public:
    //The default constructor. It does not currently do anything.
    Level();

    //The destructor frees all of the level files that need freeing.
    ~Level();

    //This function loads whatever level is needed from the corresponding level file.
    void load_level();

    //new_level() takes the dimensions for the new level in tiles.
    void new_level(int get_x,int get_y);

    void save_level(bool autosave);

    void resize_level(int get_x,int get_y);

    void new_random_level(int get_x,int get_y);

    bool room_overlaps_room(std::vector<Room> rooms,int room_index);

    void draw_line(int start_x,int start_y,int end_x,int end_y);

    bool new_map;

    //The dimensions of the level map. This is public because the Player class needs to know what the outer dimensions of the level are, so the players can be held within the
    //boundaries of the map.
    int level_x,level_y;

    int facing;

    bool persistent_level_data;

    bool night;

    bool deadly_water;

    std::vector<Background_Layer> background_layers;

    //The current level's tile array.
    std::vector< std::vector<Tile> > tile_array;

    //The current level's background tile array.
    std::vector< std::vector<Tile_Background> > tile_background_array;
};

#endif
