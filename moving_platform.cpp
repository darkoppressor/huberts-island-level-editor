/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "moving_platform.h"
#include "render.h"
#include "world.h"

using namespace std;

Moving_Platform::Moving_Platform(double get_move_speed,double get_start_x,double get_start_y,double get_end_x,double get_end_y,vector<moving_platform_waypoint> get_waypoints,bool get_active,bool get_round_trip,bool get_movement_type,short get_identifier){
    //Variables:

    ///This needs to be non-zero, or there will be problems loading these from the level file.
    move_speed=get_move_speed;

    start_point_x=get_start_x;
    start_point_y=get_start_y;

    x=start_point_x;
    y=start_point_y;

    end_point_x=get_end_x;
    end_point_y=get_end_y;

    waypoints=get_waypoints;

    w=MOVING_PLATFORM_SIZE;
    h=MOVING_PLATFORM_SIZE;

    identifier=get_identifier;

    active=get_active;

    round_trip=get_round_trip;

    movement_type=get_movement_type;

    frame=0;
    frame_counter=0;

    for(short i=0;i<3;i++){
        sprites_moving_platforms[i].x=0;
        sprites_moving_platforms[i].y=10*i;
        sprites_moving_platforms[i].w=58;
        sprites_moving_platforms[i].h=10;
    }
}

void Moving_Platform::render(bool active_platform){
    //If the moving platforms layer is currently being displayed.
    if(brush.layer_moving_platforms){
        //If the platform is in camera bounds, render the platform.
        if(x>=brush.camera.x-w && x<=brush.camera.x+brush.camera.w && y>=brush.camera.y-h && y<=brush.camera.y+brush.camera.h){
            render_sprite((int)(x-brush.camera.x),(int)(y-brush.camera.y),64,30,sprite_sheet_moving_platform,&sprites_moving_platforms[0]);
            if(!active_platform){
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),16,10,0.50,1.0,0.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_moving_platforms[0].w,sprites_moving_platforms[0].h,0.25,1.0,0.0,0.0);
            }
            else{
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),16,10,0.50,0.0,1.0,0.0);
                render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),sprites_moving_platforms[0].w,sprites_moving_platforms[0].h,0.25,0.0,1.0,0.0);
            }
        }

        //If the platform is active, render its end point and waypoints.
        if(active_platform){
            if(end_point_x>=brush.camera.x-w && end_point_x<=brush.camera.x+brush.camera.w && end_point_y>=brush.camera.y-h && end_point_y<=brush.camera.y+brush.camera.h){
                render_sprite((int)(end_point_x-brush.camera.x),(int)(end_point_y-brush.camera.y),64,30,sprite_sheet_moving_platform,&sprites_moving_platforms[2]);
                render_rectangle((int)(end_point_x-brush.camera.x),(int)(end_point_y-brush.camera.y),16,10,0.50,0.0,1.0,0.0);
                render_rectangle((int)(end_point_x-brush.camera.x),(int)(end_point_y-brush.camera.y),sprites_moving_platforms[0].w,sprites_moving_platforms[0].h,0.25,0.0,1.0,0.0);
            }

            //
            string string_waypoint="";
            for(int i=0;i<waypoints.size();i++){
                if(waypoints[i].x>=brush.camera.x-w && waypoints[i].x<=brush.camera.x+brush.camera.w && waypoints[i].y>=brush.camera.y-h && waypoints[i].y<=brush.camera.y+brush.camera.h){
                    render_sprite((int)(waypoints[i].x-brush.camera.x),(int)(waypoints[i].y-brush.camera.y),64,30,sprite_sheet_moving_platform,&sprites_moving_platforms[1]);
                    render_rectangle((int)(waypoints[i].x-brush.camera.x),(int)(waypoints[i].y-brush.camera.y),16,10,0.50,0.0,1.0,0.0);
                    render_rectangle((int)(waypoints[i].x-brush.camera.x),(int)(waypoints[i].y-brush.camera.y),sprites_moving_platforms[0].w,sprites_moving_platforms[0].h,0.25,0.0,1.0,0.0);
                    ss.clear();ss.str("");ss<<i;string_waypoint=ss.str();
                    font.show((int)(waypoints[i].x-brush.camera.x),(int)(waypoints[i].y-brush.camera.y),string_waypoint,COLOR_WHITE);
                }
            }
        }

        //If a trigger is selected.
        if(brush.current_trigger!=-1){
            for(int i=0;i<vector_triggers[brush.current_trigger].targets.size();i++){
                if(vector_triggers[brush.current_trigger].targets[i].type==0 && vector_triggers[brush.current_trigger].targets[i].identifier==identifier){
                    render_rectangle((int)(x-brush.camera.x),(int)(y-brush.camera.y),64,10,0.30,0.0,0.0,1.0);
                    break;
                }
            }
        }
    }
}
