/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "quit.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "world.h"

using namespace std;

void quit_game(){
    //Display the quit confirmation dialog box.
    /**if(brush.edit_input!=6){
        brush.edit_input=6;
    }

    //Actually quit.
    else{*/
        //Autosave the level.
        ///I should replace this with a timed autosave.
        ///level.save_level(true);

        //If any music is playing, fade it out and then stop it.
        while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()){
            SDL_Delay(100);
        }
        Mix_HaltMusic();

        SDL_EnableUNICODE(SDL_DISABLE);/**Enable unicode*/

        //This function frees any dynamically allocated memory that was loaded in load_world().
        unload_world();

        //Close the audio subsystem.
        Mix_CloseAudio();

        //Shutdown SDL.
        SDL_Quit();

        //Print a success message to stdout.txt.
        printf("Exited successfully!\n");

        //Tell the program to exit successfully.
        exit(EXIT_SUCCESS);
    ///}
}
