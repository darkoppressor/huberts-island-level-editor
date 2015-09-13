/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "background_layer.h"

using namespace std;

Background_Layer::Background_Layer(double get_start_y,double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y){
    start_y=get_start_y;

    width=get_width;
    height=get_height;

    parallax_speed_x=get_parallax_speed_x;
    parallax_speed_y=get_parallax_speed_y;
}
