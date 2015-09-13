/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "main.h"
#include "world.h"
#include "update.h"
#include "timer.h"

#ifdef GAME_OS_OSX
    #include <CoreFoundation/CoreFoundation.h>
#endif

using namespace std;

void game_loop(){
    //Set the key repeat delay to instant, and the key repeat rate to be extremely fast(almost instant).
    SDL_EnableKeyRepeat(1,0);

    //This function loads any textures, fonts, etc. that are not loaded in their respective classes. It then loads the first level.
    load_world();

    //Load the level.
    level.load_level();

    //The number of logic updates allowed each second.
    const double UPDATE_LIMIT=60;

    //Updates will be handled in SKIP_TICKS-sized chunks.
    //For instance, if UPDATE_LIMIT == 60, then SKIP_TICKS will be 16.66667, which is 1/60 of a second.
    const double SKIP_TICKS=1000/UPDATE_LIMIT;

    //The maximum number of frames to be skipped.
    const double MAX_FRAMESKIP=5;

    //In our logic update while() loop, we consume SKIP_TICKS sized chunks of time, which are added to next_game_tick.
    Uint32 next_game_tick=SDL_GetTicks();

    //The number of logic updates that have occured since the last render.
    int number_of_updates=0;

    //Declare and start the frame rate timer. This keeps track of the frame rate, as well as milliseconds spent per frame.
    //See the explanation at the beginning of the game loop for more details.
    Timer timer_frame_rate;
    timer_frame_rate.start();

    //This boolean variable is simply used to keep the game loop going indefinitely.
    bool loop=true;

    //The following three variables are used for displaying the FPS to the player.
    //This integer keeps track of the number of frame each second. Every time a second has passed, frame_count's value is given to frame_rate, and frame_count is reset to 0.
    int frame_count=0;

    //At any given time (after the first second of runtime), frame_rate represents the number of frames that were rendered in the past second.
    int frame_rate=0;

    //For added performance information, ms_per_frame takes the FPS value each second and converts it to the number of milliseconds spent on each frame in the past second.
    double ms_per_frame=0;

    //Here we have the game loop. Only when loop==false will this while() loop end. This should never happen, as when the game is closed, quit_game() should be called.
    while(loop){
        //If at least a second(preferably exactly one second) has passed since the last restart of the frame rate timer, we do several things.
        //First, we set the frame rate to the current frame count, which has been counting up since the last timer restart.
        //Second, we reset the frame count to 0, to count the number of frames succesfully completed in the next second.
        //Third, we set the milliseconds per frame to 1000/our current frame rate. Since our frame rate is in seconds, this gives us the number of milliseconds being spent on
        //each frame. Finally, we restart the frame rate timer.
        if(timer_frame_rate.get_ticks()>=1000){
            frame_rate=frame_count;
            frame_count=0;
            ms_per_frame=1000.0f/frame_rate;
            timer_frame_rate.start();
        }

        //Here we are just incrementing the frame counter, which gives us the frames per second in the above if() statement.
        frame_count++;

        //For our game loop, we first update game logic, and then render. The two are kept separate and independent.

        //First we reset our updates counter to 0.
        number_of_updates=0;

        //We consume SKIP_TICKS sized chunks of time, which ultimately translates to updating the logic UPDATE_LIMIT times a second.
        //We also check to see if we've updated logic MAX_FRAMESKIP times without rendering, at which point we render.
        while(SDL_GetTicks()>next_game_tick && number_of_updates<MAX_FRAMESKIP){
            //We are doing another game logic update.
            //If this reaches MAX_FRAMESKIP, we will render.
            number_of_updates++;

            //Consume another SKIP_TICKS sized chunk of time.
            next_game_tick+=SKIP_TICKS;

            //Now we update the game logic:

            //First, we check for input from the player.
            input();

            //Then, we move all of the objects.
            movement();

            //Once everything has had its chance to move, we handle events (collision detection).
            events();

            //Handle camera movement.
            camera();
        }

        //Now that we've handled logic updates, we do our rendering.

        //Finally, we render everything that is currently within the camera bounds to the screen.
        //We pass frame_rate and ms_per_frame so they can be displayed onscreen.
        render(frame_rate,ms_per_frame);
    }
}

//Apparently, SDL likes main() to take these arguments, so that is what we will do.
int main(int argc, char* args[]){
    #ifdef GAME_OS_OSX
        //Set the working directory to the Resources directory of our bundle.
        char path[PATH_MAX];
        CFURLRef url=CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        CFURLGetFileSystemRepresentation(url,true,(uint8_t*)path,PATH_MAX);
        CFRelease(url);
        chdir(path);
    #endif

    time_t seconds;
    uint32_t random_seed=(uint32_t)time(&seconds);
    rng.mrand_main.seed(random_seed);
    rng.mrand_render.seed(random_seed);

    //Initialize the various subsystems.
    if(main_window.init()==false){
        return 1;
    }

    //Begin the game by entering the game loop.
    game_loop();
    return 0;
}
