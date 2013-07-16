/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "world.h"
#include "render.h"

using namespace std;

//Create the main window.
Window main_window;

Random_Number_Generator rng;

//All of the other variables that are not defined in their own classes are defined here.

//We use this to poll for events when getting input.
SDL_Event event;

//The game's bitmap font.
BitmapFont font;

//String stream used to easily render ints and stuff as strings.
stringstream ss("");

//ss passes its information into this string for output.
string msg="";

//All of the world variables are defined here. These include the level object (which stores much of the level's information, and has a function that handles loading of levels),
//and the vectors that hold players, enemies, etc.

//All of the level's information is stored here.
Level level;

//This vector holds the players.
Brush brush;

//This vector holds the items in the current level.
vector<Item> vector_items;

//This vector holds the npcs in the current level.
vector<Npc> vector_npcs;

//This vector holds the moving platforms in the current level.
vector<Moving_Platform> vector_moving_platforms;

//This vector holds the triggers in the current level.
vector<Trigger> vector_triggers;

vector<Door> vector_doors;

vector<Trap> vector_traps;

vector<Sign> vector_signs;

vector<Spring> vector_springs;

vector<Booster> vector_boosters;

//Here we define the texture files. These need to be available to a number of objects, as the objects that use them are located in vectors. Thus, we simply define
//each such texture here so there are no difficulties.
GLuint sprite_sheet_brush;
GLuint sprite_sheet_items;
GLuint sprite_sheet_level_items;
GLuint sprite_sheet_npcs;
GLuint tiles;
GLuint tiles_background;
GLuint worldmap_tiles;
GLuint water_tiles;
GLuint background_static;
GLuint grid;
GLuint grid_items;
GLuint overlays;
GLuint sprite_sheet_moving_platform;
GLuint sprite_sheet_doors;
GLuint sprite_sheet_door_standard;
GLuint sprite_sheet_triggers;
GLuint sprite_sheet_traps;
GLuint sprite_sheet_signs;
GLuint sprite_sheet_spring;
GLuint sprite_sheet_booster;

//Here we define the sound files.
Mix_Chunk *sound_camera_unlock=NULL;
Mix_Chunk *sound_camera_lock=NULL;
Mix_Chunk *sound_camera_move=NULL;

//This function is called at the beginning of the game.
void load_world(){
    //Generate the font.
    font.build_font("data/images/bitmap_font.png");

    //Load the textures.
    sprite_sheet_brush=load_texture("data/images/sprite_sheet_brush.png");
    sprite_sheet_items=load_texture("data/images/sprite_sheet_items.png");
    sprite_sheet_level_items=load_texture("data/images/sprite_sheet_level_items.png");
    sprite_sheet_npcs=load_texture("data/images/sprite_sheet_npcs.png");
    tiles=load_texture("data/images/tiles.png");
    tiles_background=load_texture("data/images/tiles_background.png");
    worldmap_tiles=load_texture("data/images/worldmap_tiles.png");
    water_tiles=load_texture("data/images/water_tiles.png");
    background_static=load_texture("data/images/background_static.png");
    grid=load_texture("data/images/grid.png");
    grid_items=load_texture("data/images/grid_items.png");
    overlays=load_texture("data/images/overlays.png");
    sprite_sheet_moving_platform=load_texture("data/images/sprite_sheet_moving_platform.png");
    sprite_sheet_doors=load_texture("data/images/sprite_sheet_doors.png");
    sprite_sheet_door_standard=load_texture("data/images/sprite_sheet_door_standard.png");
    sprite_sheet_triggers=load_texture("data/images/sprite_sheet_triggers.png");
    sprite_sheet_traps=load_texture("data/images/sprite_sheet_traps.png");
    sprite_sheet_signs=load_texture("data/images/sprite_sheet_sign.png");
    sprite_sheet_spring=load_texture("data/images/sprite_sheet_spring.png");
    sprite_sheet_booster=load_texture("data/images/sprite_sheet_booster.png");

    //Load the sound effects.
    sound_camera_unlock=Mix_LoadWAV("data/sounds/camera_unlock.wav");
    sound_camera_lock=Mix_LoadWAV("data/sounds/camera_lock.wav");
    sound_camera_move=Mix_LoadWAV("data/sounds/camera_move.wav");
}

//This is called in quit_game(). It frees all of the memory allocated in load_world().
void unload_world(){
    //The font has its own deconstructor to handle its texture.

    //Delete the textures:
    glDeleteTextures(1,&sprite_sheet_brush);
    glDeleteTextures(1,&sprite_sheet_items);
    glDeleteTextures(1,&sprite_sheet_level_items);
    glDeleteTextures(1,&sprite_sheet_npcs);
    glDeleteTextures(1,&tiles);
    glDeleteTextures(1,&tiles_background);
    glDeleteTextures(1,&worldmap_tiles);
    glDeleteTextures(1,&water_tiles);
    glDeleteTextures(1,&background_static);
    glDeleteTextures(1,&grid);
    glDeleteTextures(1,&grid_items);
    glDeleteTextures(1,&overlays);
    glDeleteTextures(1,&sprite_sheet_moving_platform);
    glDeleteTextures(1,&sprite_sheet_doors);
    glDeleteTextures(1,&sprite_sheet_door_standard);
    glDeleteTextures(1,&sprite_sheet_triggers);
    glDeleteTextures(1,&sprite_sheet_traps);
    glDeleteTextures(1,&sprite_sheet_signs);
    glDeleteTextures(1,&sprite_sheet_spring);
    glDeleteTextures(1,&sprite_sheet_booster);

    //Free the sound effects.
    Mix_FreeChunk(sound_camera_unlock);
    Mix_FreeChunk(sound_camera_lock);
    Mix_FreeChunk(sound_camera_move);
}
