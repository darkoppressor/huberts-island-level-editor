/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef brush_h
#define brush_h

#include "tile_sprites.h"
#include "item_dimensions.h"
#include "npc_dimensions.h"
#include "trap_dimensions.h"

#include <string>
#include <vector>

#include <SDL.h>

class Brush{
    private:
    //We will use this to hold the keystates, which will be given to us by SDL.
    Uint8 *keystates;

    //These constants are used for move states, shoot states, animation states, etc.
    int LEFT,UP,RIGHT,DOWN,LEFT_UP,RIGHT_UP,RIGHT_DOWN,LEFT_DOWN;

    //Current movement state of the player, current frame of animation, and current direction the player is facing.
    int move_state;

    SDL_Rect sprites_box[11];

    public:
    //The constructor takes starting coordinates.
    Brush();

    std::string return_npc_name(short i);

    ///
    bool is_tile_ok(int int_x,int int_y);

    //Get input.
    void handle_input();

    //Move the player.
    void move();

    //Handle events, such as collision checks.
    void handle_events();

    //Handle camera movement.
    void set_camera();

    //Render the background.
    void render_background();

    //Render the sprite.
    void render();

    void reset_copy();

    //Run when creating or deleting any kind of object that can be targeted by a trigger.
    void generate_identifiers(short deleted_type=0,short deleted_identifier=-1);

    void create_item(short i,short n,short type);
    void create_npc(short i,short n,short type);

    short number_of_height_masks;

    //The current coordinates of the player.
    double x,y;

    //The dimensions of the player. These are used for collision detection.
    double w,h;

    //The coordinates and dimensions of the camera.
    struct kevin_rect{
        double x;
        double y;
        double w;
        double h;
    };
    kevin_rect camera;

    SDL_Rect sprites_overlays[10];

    //This SDL_Rect holds the tile sprites, for displaying them in windows and such in a list.
    SDL_Rect sprites_tiles[TILE_SPRITES];

    SDL_Rect worldmap_sprites_tiles[WORLDMAP_TILES];

    SDL_Rect sprites_tiles_background[BACKGROUND_TILES];

    bool desert_addition;

    //Editing variables:

    //The display status of each layer.
    bool layer_tiles;
    bool layer_items;
    bool layer_tiles_background;
    bool layer_npcs;
    bool layer_moving_platforms;
    bool layer_triggers;
    bool layer_doors;
    bool layer_traps;
    bool layer_signs;
    bool layer_springs;
    bool layer_boosters;

    //The layer currently being edited.
    //0 = terrain tiles.
    //1 = items.
    //2 = background tiles.
    //3 = npcs.
    //4 = moving platforms.
    //5 = triggers.
    //6 = doors.
    //7 = traps.
    //8 = signs.
    //9 = springs.
    //10 = boosters.
    int layer_current;

    //The current item ready to be placed.
    int item_current;

    //The currently selected item.
    int item_selected;

    //The current npc ready to be placed.
    int npc_current;

    //The current trap ready to be placed.
    int trap_current;

    //0 = none
    //1 = moving platform move_speed
    //2 = done editing a moving platforms's move_speed
    //3 = creating a new level
    //4 = sprite
    //5 = done editing a tile's sprite
    //6 = quit confirmation
    //7 = new confirmation
    //8 = resize confirmation
    //9 = save confirmation
    //10 = new random confirmation
    //11 = editing a tile's sloping ground angle
    //12 = done editing a tile's sloping ground angle
    //13 = editing trigger's repeat time
    //14 = done editing trigger's repeat time
    //15 = editing a tile's height mask
    //16 = done editing a tile's height mask
    //17 = editing a sign's message
    //18 = done editing a sign's message
    //19 = item goal_level_to_load
    //20 = done editing an item's goal_level_to_load
    //21 = resizing the level
    //22 = editing a spring's speed
    //23 = done editing a spring's speed
    //24 = editing a booster's speed
    //25 = done editing a booster's speed
    int edit_input;

    //If true, the new level will be random.
    //If false, it will not.
    bool new_level_is_random;

    //If true, the brush will move significantly faster.
    bool speed_mode;

    ///I don't think these are used.
    int edit_solidity;

    ///This one is now used when editing tile sprites AND when editing angle and height mask.
    ///I might want to rename this to something like edit_int or edit_number or whatever.
    int edit_sprite;

    double edit_move_speed;

    int edit_goal_level_to_load;

    short edit_repeat_time;

    int edit_new_x;
    int edit_new_y;

    std::string edit_string;

    //Hardcoded height masks.
    ///short height_masks[number_of_height_masks][TILE_W];
    std::vector< std::vector<short> > height_masks;

    //The currently selected moving platform.
    int current_platform;

    //The currently selected trigger.
    int current_trigger;

    //The currently selected door.
    int current_door;

    //The currently selected trap.
    int current_trap;

    //The currently selected sign.
    int current_sign;

    int current_spring;

    int current_booster;

    //Options.
    bool option_display_help;
    bool option_display_overlays;
    bool option_display_grid;

    short MAX_BRUSH_SIZE;

    //Copy/paste variables.
    int copy_solidity[30][30];
    int copy_sprite[30][30];
    int copy_special[30][30];
    bool copy_foreground[30][30];
    short copy_slope[30][30];
    short copy_height_mask[30][30][TILE_W];

    int copy_item_type[30][30];
    int copy_npc_type[30][30];

    double window_big_x,window_big_y;
    bool window_big_moving;

    //The number of hazard tiles in this level.
    //-1 means the number has not been updated yet.
    int hazard_tiles;

    int amount_leaves;
    int amount_cheese;
    int amount_ammo;
    int amount_spawnpoint;
    int amount_checkpoint;
    int amount_endpoint;
    int amount_swimming_gear;
    int amount_key_red;
    int amount_key_blue;
    int amount_key_green;
    int amount_key_yellow;
    int amount_key_orange;
    int amount_key_purple;
    int amount_towel;
    int amount_key_gray;
    int amount_key_brown;
    int amount_key_black;
    int amount_key_pink;
    int amount_key_cyan;
    int amount_sink;
    int amount_j_balloon;
    int amount_ammo_barrel;
    int amount_candy;
    int amount_suit_deadly_water;
    int amount_suit_sharp;
    int amount_suit_banana;
    int amount_shot_homing;
    int amount_translator;
    int amount_j_wing;

    int amount_npcs[44];

    int amount_moving_platforms;

    int amount_triggers;

    int amount_doors;
    bool door_local_0;
    bool door_local_1;
    bool door_local_2;
    bool door_local_3;
    bool door_global_0;
    bool door_global_1;
    bool door_global_2;
    bool door_global_3;
    bool door_global_4;
    bool door_global_5;
    bool door_global_6;
    bool door_global_7;
    bool door_global_8;
    bool door_global_9;
    bool door_global_10;
    bool door_global_11;

    int amount_trap_fire;

    bool WORLD_MAP;

    double full_brush_w;
    double full_brush_h;
};

#endif
