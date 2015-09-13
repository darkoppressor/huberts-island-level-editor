/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef font_h
#define font_h

#include <SDL.h>
#include <string>
#include <SDL_opengl.h>

class BitmapFont{
    private:
    //The font surface.
    GLuint bitmap_font_texture;

    //The width and height of the entire texture.
    double w,h;

    //The individual characters in the surface.
    SDL_Rect chars[256];

    public:
    //The default constructor.
    BitmapFont();

    //The destructor deletes the font texture.
    ~BitmapFont();

    //Generates the font.
    void build_font(std::string font_location);

    //Shows the text.
    void show(double x,double y,std::string text,short font_color=3,double opacity=1.0);
};

#endif
