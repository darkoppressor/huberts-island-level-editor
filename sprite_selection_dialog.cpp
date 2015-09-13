/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sprite_selection_dialog.h"

using namespace std;

short set_tile_row(short sprite_number){
    short row=0;

    if(sprite_number>=37 && sprite_number<=73){
        row=1;
    }
    else if(sprite_number>=74 && sprite_number<=110){
        row=2;
    }
    else if(sprite_number>=111 && sprite_number<=147){
        row=3;
    }
    else if(sprite_number>=148 && sprite_number<=184){
        row=4;
    }
    else if(sprite_number>=185 && sprite_number<=221){
        row=5;
    }
    else if(sprite_number>=222 && sprite_number<=258){
        row=6;
    }
    else if(sprite_number>=259 && sprite_number<=295){
        row=7;
    }
    else{
        row=0;
    }

     return row;
}

short set_tile_column(short sprite_number){
    short column=0;

    if(sprite_number<37){
        column=sprite_number;
    }
    else if(sprite_number>=37 && sprite_number<=73){
        column=sprite_number-37;
    }
    else if(sprite_number>=74 && sprite_number<=110){
        column=sprite_number-74;
    }
    else if(sprite_number>=111 && sprite_number<=147){
        column=sprite_number-111;
    }
    ///
    else if(sprite_number>=148 && sprite_number<=184){
        column=sprite_number-148;
    }
    else if(sprite_number>=185 && sprite_number<=221){
        column=sprite_number-185;
    }
    else if(sprite_number>=222 && sprite_number<=258){
        column=sprite_number-222;
    }
    else if(sprite_number>=259 && sprite_number<=295){
        column=sprite_number-259;
    }
    else{
        column=0;
    }

    return column;
}
