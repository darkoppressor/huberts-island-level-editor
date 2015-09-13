/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef sign_h
#define sign_h

#include <string>

#include <SDL.h>

class Sign{
    private:
    SDL_Rect sprite_sign[2];

    public:
    Sign(double get_x,double get_y,std::string get_message,short get_type,short get_font_type);

    void render(bool active);

    std::string message;

    double x,y;

    double w,h;

    short type;

    short font_type;
};

#endif
