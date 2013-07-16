/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef world_h
#define world_h

#include <string>
#include <sstream>

#include "level.h"
#include "font.h"
#include "tile.h"
#include "brush.h"
#include "item.h"
#include "npc.h"
#include "window.h"
#include "random_number_generator.h"
#include "moving_platform.h"
#include "trigger.h"
#include "door.h"
#include "trap.h"
#include "sign.h"
#include "spring.h"
#include "booster.h"

extern Window main_window;

//Colors:

const short COLOR_RAINBOW=0;

const short COLOR_BLACK=1;

const short COLOR_GRAY=2;

const short COLOR_WHITE=3;

const short COLOR_BROWN=4;
const short COLOR_DARK_BROWN=5;
const short COLOR_LIGHT_BROWN=6;
const short COLOR_SAND=7;

const short COLOR_YELLOW=8;
const short COLOR_SYSTEM=9;
const short COLOR_GOLD=10;

const short COLOR_ORANGE=11;
const short COLOR_PUMPKIN=12;

const short COLOR_RED=13;
const short COLOR_DARK_RED=14;
const short COLOR_FLAME=15;

const short COLOR_PINK=16;
const short COLOR_ROSE=17;
const short COLOR_SHOCKING_PINK=18;

const short COLOR_PURPLE=19;
const short COLOR_VIOLET=20;
const short COLOR_INDIGO=21;

const short COLOR_BLUE=22;
const short COLOR_SKY_BLUE=23;
const short COLOR_UN_BLUE=24;

const short COLOR_GREEN=25;
const short COLOR_JUNGLE=26;
const short COLOR_SPRING=27;

const short COLOR_DIRT=28;

const short COLOR_WATER_TROPICAL_SHALLOW=29;
const short COLOR_WATER_TROPICAL_DEEP=30;

extern SDL_Event event;

extern BitmapFont font;

extern Random_Number_Generator rng;

extern std::stringstream ss;
extern std::string msg;

extern Level level;

extern Brush brush;
extern std::vector<Item> vector_items;
extern std::vector<Npc> vector_npcs;
extern std::vector<Moving_Platform> vector_moving_platforms;
extern std::vector<Trigger> vector_triggers;
extern std::vector<Door> vector_doors;
extern std::vector<Trap> vector_traps;
extern std::vector<Sign> vector_signs;
extern std::vector<Spring> vector_springs;
extern std::vector<Booster> vector_boosters;

extern GLuint sprite_sheet_brush;
extern GLuint sprite_sheet_items;
extern GLuint sprite_sheet_level_items;
extern GLuint sprite_sheet_npcs;
extern GLuint tiles;
extern GLuint tiles_background;
extern GLuint worldmap_tiles;
extern GLuint water_tiles;
extern GLuint background_static;
extern GLuint grid;
extern GLuint grid_items;
extern GLuint overlays;
extern GLuint sprite_sheet_moving_platform;
extern GLuint sprite_sheet_doors;
extern GLuint sprite_sheet_door_standard;
extern GLuint sprite_sheet_triggers;
extern GLuint sprite_sheet_traps;
extern GLuint sprite_sheet_signs;
extern GLuint sprite_sheet_spring;
extern GLuint sprite_sheet_booster;

extern Mix_Chunk *sound_camera_unlock;
extern Mix_Chunk *sound_camera_lock;
extern Mix_Chunk *sound_camera_move;

void load_world();

void unload_world();

#endif
