/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef enumerations_h
#define enumerations_h

//**************************//
// Tile Special Properties: //
//**************************//

enum{
    TILE_SPECIAL_NONE,
    TILE_SPECIAL_CLIMBABLE,
    TILE_SPECIAL_CLIMBABLE_TOP,
    TILE_SPECIAL_HAZARD,
    TILE_SPECIAL_WATER,
    TILE_SPECIAL_MIRROR,
    TILE_SPECIAL_ITEM_HOLDER
};

//***************************//
// Tile Solidity Properties: //
//***************************//

enum{
    TILE_SOLIDITY_PASSABLE,
    TILE_SOLIDITY_SOLID,
    TILE_SOLIDITY_CLOUD
};

//********//
// Items: //
//********//

enum{
    ITEM_BEGIN,
    ITEM_NONE=ITEM_BEGIN,
    ITEM_LEAF,
    ITEM_CHEESE,
    ITEM_AMMO,
    ITEM_SPAWNPOINT,
    ITEM_CHECKPOINT,
    ITEM_ENDPOINT,
    ITEM_SWIMMING_GEAR,
    ITEM_KEY_RED,
    ITEM_KEY_BLUE,
    ITEM_KEY_GREEN,
    ITEM_KEY_YELLOW,
    ITEM_KEY_ORANGE,
    ITEM_KEY_PURPLE,
    ITEM_TOWEL,
    ITEM_SINK,
    ITEM_J_BALLOON,
    ITEM_AMMO_BARREL,
    ITEM_CANDY,
    ITEM_KEY_GRAY,
    ITEM_KEY_BROWN,
    ITEM_KEY_BLACK,
    ITEM_KEY_PINK,
    ITEM_KEY_CYAN,
    ITEM_SUIT_DEADLY_WATER,
    ITEM_SUIT_SHARP,
    ITEM_SUIT_BANANA,
    ITEM_SHOT_HOMING,
    ITEM_TRANSLATOR,
    ITEM_J_WING,
    ITEM_END
};

//*********//
// Colors: //
//*********//

/**enum{
    COLOR_RAINBOW,

    COLOR_BLACK,

    COLOR_GRAY,

    COLOR_WHITE,

    COLOR_BROWN,
    COLOR_DARK_BROWN,
    COLOR_COPPER,
    COLOR_BRONZE,
    COLOR_SEPIA,
    COLOR_SANDY_BROWN,
    COLOR_WHEAT,
    COLOR_TAN,
    COLOR_PALE_BROWN,

    COLOR_YELLOW,
    COLOR_SYSTEM,
    COLOR_GOLD,

    COLOR_ORANGE,
    COLOR_PUMPKIN,

    COLOR_RED,
    COLOR_DARK_RED,
    COLOR_FLAME,

    COLOR_PINK,
    COLOR_ROSE,
    COLOR_SHOCKING_PINK,

    COLOR_PURPLE,
    COLOR_VIOLET,
    COLOR_INDIGO,

    COLOR_BLUE,
    COLOR_SKY_BLUE,
    COLOR_UN_BLUE,

    COLOR_GREEN,
    COLOR_JUNGLE,
    COLOR_SPRING
};*/

//*************//
// Directions: //
//*************//

enum{
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    LEFT_UP,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    //This constant is used as a cam_state value, to tell the camera whether it is free or attached to a player.
    CAM_STICKY
};

#endif
