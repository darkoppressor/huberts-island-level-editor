/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef moving_platform_h
#define moving_platform_h

#include <vector>

#include <SDL.h>

#include "moving_platform_waypoint.h"

class Moving_Platform{
    private:
    //Editor only.
    SDL_Rect sprites_moving_platforms[3];

    public:
    Moving_Platform(double get_move_speed,double get_start_x,double get_start_y,double get_end_x,double get_end_y,std::vector<moving_platform_waypoint> get_waypoints,bool get_active,bool get_round_trip,bool get_movement_type,short get_identifier);

    //Render the sprite.
    void render(bool active_platform);

    //The speed at which the platform moves.
    double move_speed;

    //If active is true, the platform moves, checks for collisions, etc.
    bool active;

    //If true, platform continues moving when it reaches its goal.
    //If false, platform becomes inactive when it reaches its goal.
    bool round_trip;

    //False = Back and forth. When end point is reached, platform returns to start point via waypoints. When start point is reached, platform returns to end point via waypoints.
    //True = Circular. When end point is reached, platform returns to start point directly. When start point is reached, platform returns to end point via waypoints.
    bool movement_type;

    //
    double start_point_x,start_point_y;

    //
    double end_point_x,end_point_y;

    //
    std::vector<moving_platform_waypoint> waypoints;

    //
    short identifier;

    //The current coordinates of the platform.
    double x,y;

    //The dimensions of the platform.
    double w,h;

    //Animation frame and frame counter.
    short frame,frame_counter;
};

#endif
