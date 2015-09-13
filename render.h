/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef render_h
#define render_h

#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

SDL_Surface* optimize_image_opengl(SDL_Surface* surface);

//Load an image that will be used to create an OpenGL texture.
SDL_Surface* load_image_opengl(std::string filename);

GLuint surface_to_texture(SDL_Surface* surface);

//Call this when loading image files to be used as textures throughout the program.
GLuint load_texture(std::string filename);

void render_texture(double x,double y,double w,double h,GLuint source,double opacity=1.0);

void render_sprite(double x,double y,double w,double h,GLuint source,SDL_Rect* texture_clip=NULL,double opacity=1.0,bool flip=false,double angle=0.0);

void render_rectangle(double x,double y,double w,double h,double opacity=1.0,double r=1.0,double g=1.0,double b=1.0);

void render_font(double x,double y,double w,double h,GLuint source,SDL_Rect* texture_clip,short font_color=1,double opacity=1.0);

#endif
