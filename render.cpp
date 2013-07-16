/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "render.h"
#include "world.h"

using namespace std;

SDL_Surface* optimize_image_opengl(SDL_Surface* surface){
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette=0;
    RGBAFormat.colorkey=0;
    RGBAFormat.alpha=0;
    RGBAFormat.BitsPerPixel=32;
    RGBAFormat.BytesPerPixel=4;

    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        RGBAFormat.Rmask=0xFF000000; RGBAFormat.Rshift=0; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x00FF0000; RGBAFormat.Gshift=8; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x0000FF00; RGBAFormat.Bshift=16; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0x000000FF; RGBAFormat.Ashift=24; RGBAFormat.Aloss=0;
    }
    else{
        RGBAFormat.Rmask=0x000000FF; RGBAFormat.Rshift=24; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x0000FF00; RGBAFormat.Gshift=16; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x00FF0000; RGBAFormat.Bshift=8; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0xFF000000; RGBAFormat.Ashift=0; RGBAFormat.Aloss=0;
    }

    SDL_Surface* conv=SDL_ConvertSurface(surface,&RGBAFormat,SDL_SWSURFACE);

    return conv;
}

SDL_Surface* load_image_opengl(string filename){
    SDL_Surface* loaded_image=0;
    SDL_Surface* optimized_image=0;

    //Load the image.
    loaded_image=IMG_Load(filename.c_str());

    if(loaded_image!=0){
        optimized_image=optimize_image_opengl(loaded_image);
        SDL_FreeSurface(loaded_image);
    }

    //Return the optimized image.
    return optimized_image;
}

GLuint surface_to_texture(SDL_Surface* surface){
    //This texture will be returned at the end of this function.
    GLuint texture;

    //Have OpenGL generate a texture object handle for us.
    glGenTextures(1,&texture);

    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,texture);

    //Set the texture's properties:
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);///GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);///GL_LINEAR

    //Edit the texture object's image data using the information our image gives us:
    glTexImage2D(GL_TEXTURE_2D,0,surface->format->BytesPerPixel,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

    return texture;
}

GLuint load_texture(string filename){
    //This will temporarily store the name of the image we are loading.
    SDL_Surface* surface=0;

    //Load the image.
    surface=load_image_opengl(filename.c_str());

    GLuint texture=surface_to_texture(surface);

    //We have copied the image data into the new texture, so delete the image data.
    SDL_FreeSurface(surface);

    //Return the new texture we have created.
    return texture;
}

//Render a full texture to the screen.
void render_texture(double x,double y,double w,double h,GLuint source,double opacity){
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,source);

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glColor4d(1.0,1.0,1.0,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(w,0,0);
    glTexCoord2d(1,1);  glVertex3d(w,h,0);
    glTexCoord2d(0,1);  glVertex3d(0,h,0);

    //End quad.
    glEnd();

    //Reset.
    glLoadIdentity();
}

//Render a sprite from a spritesheet to the screen.
void render_sprite(double x,double y,double w,double h,GLuint source,SDL_Rect* texture_clip,double opacity,bool flip,double angle){
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,source);

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glTranslated(texture_clip->w/2.0,texture_clip->h/2.0,0);

    glRotated(angle,0,0,-1);

    glTranslated(-texture_clip->w/2.0,-texture_clip->h/2.0,0);

    glColor4d(1.0,1.0,1.0,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:

    //If flip is false, just apply the texture to the screen normally.
    if(flip==false){
        //Bottom left corner.
        glTexCoord2d((texture_clip->x)/w,(texture_clip->y+texture_clip->h)/h);
        glVertex3d(0,texture_clip->h,0);

        //Bottom right corner.
        glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y+texture_clip->h)/h);
        glVertex3d(texture_clip->w,texture_clip->h,0);

        //Top right corner.
        glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y)/h);
        glVertex3d(texture_clip->w,0,0);

        //Top left corner.
        glTexCoord2d((texture_clip->x)/w,(texture_clip->y)/h);
        glVertex3d(0,0,0);
    }

    //If flip is true, flip the image on the x-axis.
    if(flip==true){
        //Bottom right corner.
        glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y+texture_clip->h)/h);

        //Bottom left corner.
        glVertex3d(0,texture_clip->h,0);

        //Bottom left corner.
        glTexCoord2d((texture_clip->x)/w,(texture_clip->y+texture_clip->h)/h);

        //Bottom right corner.
        glVertex3d(texture_clip->w,texture_clip->h,0);

        //Top left corner.
        glTexCoord2d((texture_clip->x)/w,(texture_clip->y)/h);

        //Top right corner.
        glVertex3d(texture_clip->w,0,0);

        //Top right corner.
        glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y)/h);

        //Top left corner.
        glVertex3d(0,0,0);
    }

    //End quad.
    glEnd();

    //Reset.
    glLoadIdentity();
}

void render_rectangle(double x,double y,double w,double h,double opacity,double r,double g,double b){
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glColor4d(r,g,b,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(w,0,0);
    glTexCoord2d(1,1);  glVertex3d(w,h,0);
    glTexCoord2d(0,1);  glVertex3d(0,h,0);

    //End quad.
    glEnd();

    //Reset.
    glLoadIdentity();
}

//Render text to the screen using a bitmap font.
void render_font(double x,double y,double w,double h,GLuint source,SDL_Rect* texture_clip=NULL,short font_color,double opacity){
    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    //Set the font colors.

    if(font_color==COLOR_BLACK){
        glColor4d(0.0,0.0,0.0,opacity);
    }

    else if(font_color==COLOR_GRAY){
        glColor4d(0.5,0.5,0.5,opacity);
    }

    else if(font_color==COLOR_WHITE){
        glColor4d(1.0,1.0,1.0,opacity);
    }

    else if(font_color==COLOR_BROWN){
        glColor4d(0.588235,0.294117,0.0,opacity);
    }

    else if(font_color==COLOR_DARK_BROWN){
        glColor4d(0.435294,0.207843,0.101960,opacity);
    }

    else if(font_color==COLOR_LIGHT_BROWN){
        glColor4d(0.803921,0.498039,0.196078,opacity);
    }

    else if(font_color==COLOR_SAND){
        //glColor4d(0.956862,0.761994,0.376470,opacity);
        glColor4d(0.908001,0.850002,0.704004,opacity);
    }

    else if(font_color==COLOR_YELLOW){
        glColor4d(1.0,1.0,0.0,opacity);
    }

    else if(font_color==COLOR_SYSTEM){
        glColor4d(1.0,0.847058,0.0,opacity);
    }

    else if(font_color==COLOR_GOLD){
        glColor4d(0.831372,0.686274,0.215686,opacity);
    }

    else if(font_color==COLOR_ORANGE){
        glColor4d(1.0,0.623529,0.0,opacity);
    }

    else if(font_color==COLOR_PUMPKIN){
        glColor4d(1.0,0.458823,0.094117,opacity);
    }

    else if(font_color==COLOR_RED){
        glColor4d(1.0,0.0,0.0,opacity);
    }

    else if(font_color==COLOR_DARK_RED){
        glColor4d(0.807843,0.086274,0.125490,opacity);
    }

    else if(font_color==COLOR_FLAME){
        glColor4d(0.886274,0.345098,0.133333,opacity);
    }

    else if(font_color==COLOR_PINK){
        glColor4d(1.0,0.752941,0.796078,opacity);
    }

    else if(font_color==COLOR_ROSE){
        glColor4d(1.0,0.0,0.498039,opacity);
    }

    else if(font_color==COLOR_SHOCKING_PINK){
        glColor4d(0.988235,0.058823,0.752941,opacity);
    }

    else if(font_color==COLOR_PURPLE){
        glColor4d(0.5,0.0,0.5,opacity);
    }

    else if(font_color==COLOR_VIOLET){
        glColor4d(0.545,0.0,1.0,opacity);
    }

    else if(font_color==COLOR_INDIGO){
        glColor4d(0.435,0.0,1.0,opacity);
    }

    else if(font_color==COLOR_BLUE){
        glColor4d(0.0,0.0,1.0,opacity);
    }

    else if(font_color==COLOR_SKY_BLUE){
        glColor4d(0.324999,0.754994,0.913001,opacity);
    }

    else if(font_color==COLOR_UN_BLUE){
        glColor4d(0.356862,0.572549,0.898039,opacity);
    }

    else if(font_color==COLOR_GREEN){
        glColor4d(0.0,1.0,0.0,opacity);
    }

    else if(font_color==COLOR_JUNGLE){
        glColor4d(0.160784,0.670588,0.529411,opacity);
    }

    else if(font_color==COLOR_SPRING){
        glColor4d(0.0,1.0,0.498039,opacity);
    }

    else if(font_color==COLOR_DIRT){
        glColor4d(0.705882,0.635294,0.431372,opacity);
    }

    else if(font_color==COLOR_WATER_TROPICAL_SHALLOW){
        glColor4d(0.098039,0.776470,0.992156,opacity);
    }

    else if(font_color==COLOR_WATER_TROPICAL_DEEP){
        glColor4d(0.117647,0.501960,0.886274,opacity);
    }

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:

    //Bottom left corner.
    glTexCoord2d((texture_clip->x)/w,(texture_clip->y+texture_clip->h)/h);

    //Bottom left corner.
    glVertex3d(0,texture_clip->h,0);

    //Bottom right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y+texture_clip->h)/h);

    //Bottom right corner.
    glVertex3d(texture_clip->w,texture_clip->h,0);

    //Top right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/w,(texture_clip->y)/h);

    //Top right corner.
    glVertex3d(texture_clip->w,0,0);

    //Top left corner.
    glTexCoord2d((texture_clip->x)/w,(texture_clip->y)/h);

    //Top left corner.
    glVertex3d(0,0,0);

    //End quad.
    glEnd();

    //Reset.
    glLoadIdentity();
}
