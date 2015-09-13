/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef background_layer_h
#define background_layer_h

class Background_Layer{
    private:
    public:

    Background_Layer(double get_start_y,double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y);

    //The starting y offset. Should be set so the background's bottom lines up with the ground in the level.
    double start_y;

    //The dimensions of layer images.
    double width;
    double height;

    //The rate at which the background scrolls.
    //Lower numbers scroll the background faster.
    double parallax_speed_x,parallax_speed_y;
};

#endif
