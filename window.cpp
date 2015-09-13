/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window.h"
#include "pixels.h"

#include <time.h>
#include <string>

#include "quit.h"

using namespace std;

Window::Window(){
    //Here we are going to set the resolution and color depth of our game screen.
    SCREEN_WIDTH=1280;
    SCREEN_HEIGHT=800;

    //Since we've set the screen's color depth to 0, SDL will attempt to set it to the computer's color depth when initializing.
    SCREEN_BPP=0;

    //This surface will be used as our screen.
    screen=NULL;
}

bool Window::init(){
    //Initialize all of the SDL stuff:

    //Initialize all of the standard SDL stuff, and return false if it did not initialize properly.
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        fprintf(stderr,"Unable to init SDL: %s\n",SDL_GetError());
        return false;
    }

    //Attempt to center the window on the screen.
    SDL_putenv("SDL_VIDEO_CENTERED=center");

    //Set OpenGL attributes:
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    //Set up the screen:
    ///screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE|SDL_FULLSCREEN);
    screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE);

    //Return false if the screen could not be set up.
    if(!screen){
        fprintf(stderr,"Unable to set video mode: %s\n",SDL_GetError());
        return false;
    }

    //Initialize OpenGL:
    //I am still a beginner at using OpenGL, so I'll put forth my best explanation for how things work, exactly.

    //Enable 2D textures.
    glEnable(GL_TEXTURE_2D);

    //Enable blending.
    glEnable(GL_BLEND);

    //
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //Set clear color. When the screen is cleared, it will be filled with this color.
    glClearColor(.435998,.449998,.428998,1);

    //Set the OpenGL viewport to be the same size as our screen.
    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    //Set projection:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,-1,1);

    //Initialize modelview matrix:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //If there were any errors.
    if(glGetError()!=GL_NO_ERROR){
        fprintf(stderr,"OpenGL initialization failed.\n");
        return false;
    }

    //Start up the audio system.
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){
        fprintf(stderr,"SDL_Mixer initialization failed: %s\n",SDL_GetError());
        return false;
    }

    //Set the window caption.
    SDL_WM_SetCaption("Hubert Editor","Hubert Editor");

    //Show the mouse cursor.
    SDL_ShowCursor(1);

    //Everything initialized fine, so return true.
    return true;
}

void Window::screenshot(){
    //First, we setup the filename for the screenshot.

    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);

    //Store the date and time in buff.
    strftime(buff,sizeof buff,"%Y-%A_%B_%d-%H%M.%S",tm_now);

    //Copy buff's data into the date string for use with screenshot_name below.
    string date=buff;

    //Used to store the filename of the screenshot.
    string screenshot_name;

    //Set the filename.
    screenshot_name="Screenshots/";
    screenshot_name+=date;
    screenshot_name+=".bmp";

    //Create temporary surfaces.
    SDL_Surface *surface=NULL;
    SDL_Surface *flipped_surface=NULL;

    //This will hold the pixel data from the frame buffer.
    unsigned char pixel_data[4*SCREEN_WIDTH*SCREEN_HEIGHT];

    //Read the pixels from the frame buffer and store them in pixel_data.
    glReadPixels(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GL_RGBA,GL_UNSIGNED_BYTE,pixel_data);

    uint32_t rmask,gmask,bmask,amask;

    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        rmask=0xff000000;
        gmask=0x00ff0000;
        bmask=0x0000ff00;
        amask=0x000000ff;
    }
    else{
        rmask=0x000000ff;
        gmask=0x0000ff00;
        bmask=0x00ff0000;
        amask=0xff000000;
    }

    //Create an SDL surface from this pixel data.
    surface=SDL_CreateRGBSurfaceFrom(pixel_data,SCREEN_WIDTH,SCREEN_HEIGHT,screen->format->BitsPerPixel,SCREEN_WIDTH*screen->format->BytesPerPixel,rmask,gmask,bmask,amask);

    //Create a SDL surface for holding the flipped image.
    flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,surface->format->BitsPerPixel,rmask,gmask,bmask,amask);

    //If the surface must be locked.
    if(SDL_MUSTLOCK(surface)){
        //Lock the surface.
        SDL_LockSurface(surface);
    }

    //Read the pixel data from surface and store it (flipped vertically) in flipped_surface.
    for(int x=0,rx=surface->w-1;x<surface->w;x++,rx--){
        for(int y=0,ry=surface->h-1;y<surface->h;y++,ry--){
            Uint32 pixel=surface_get_pixel(surface,x,y);
            surface_put_pixel(flipped_surface,x,ry,pixel);
        }
    }

    //If the surface had to be locked.
    if(SDL_MUSTLOCK(surface)){
        //Unlock the surface.
        SDL_UnlockSurface(surface);
    }

    //Save the flipped surface to the screenshot file.
    SDL_SaveBMP(flipped_surface,screenshot_name.c_str());

    //Free the surfaces.
    SDL_FreeSurface(flipped_surface);
    SDL_FreeSurface(surface);
}
