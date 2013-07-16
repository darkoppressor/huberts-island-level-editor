/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "font.h"
#include "render.h"

using namespace std;

BitmapFont::BitmapFont(){
    //Initialize the variables:
    bitmap_font_texture=NULL;
    w=0;
    h=0;
}

BitmapFont::~BitmapFont(){
    //When the destructor is called, delete the font texture.
    glDeleteTextures(1,&bitmap_font_texture);
}

void BitmapFont::build_font(string font_location){
    //
    bitmap_font_texture=load_texture(font_location);

    //Set the texture dimensions:
    w=4096;
    h=16;

    //Set the cell dimensions:
    double cellW=16;
    double cellH=16;

    //The current character we are setting.
    int currentChar=0;

    //Go through the cell columns.
    for(short cols=0;cols<256;cols++){
        //Set the character offset:
        chars[currentChar].x=cellW*cols;
        chars[currentChar].y=0;
        //Set the dimensions of the character:
        chars[currentChar].w=cellW;
        chars[currentChar].h=cellH;
        //Go to the next character.
        currentChar++;
    }
}

void BitmapFont::show(double x,double y,string text,short font_color,double opacity){
    //Temporary offsets.
    double X=x,Y=y;

    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,bitmap_font_texture);

    //Go through the text.
    for(int show=0;text[show]!='\0';show++){
        //Get the ASCII value of the character.
        int ascii=(unsigned char)text[show];
        if(text[show]!='\xA'){
            //Show the character.
            render_font((int)X,(int)Y,w,h,bitmap_font_texture,&chars[ascii],font_color,opacity);
            //Move over the width of the character with one pixel of padding.
            X+=chars[ascii].w-5;
        }
        else{
            Y+=chars[ascii].h+8;
            X=x;
        }
    }
}
