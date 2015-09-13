/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_h
#define window_h

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>

class Window{
    private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int SCREEN_BPP;
    SDL_Surface *screen;
    public:
    Window();
    bool init();
    void screenshot();
};

#endif
