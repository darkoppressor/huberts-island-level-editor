/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "level.h"
#include "world.h"
#include "render.h"
#include "random_number_generator.h"
#include "item_dimensions.h"
#include "npc_dimensions.h"
#include "collision.h"

#include <fstream>
#include <ctime>

#include <boost/algorithm/string.hpp>

using namespace std;

Level::Level(){
    new_map=false;

    level_x=0;
    level_y=0;

    facing=0;

    persistent_level_data=false;
    night=false;
    deadly_water=false;
}

Level::~Level(){
}

void Level::load_level(){
    //Clear the vectors. This ensures that any previous data held in the vectors is deleted, and that any memory that was allocated by the objects in the vectors is freed.
    background_layers.clear();

    vector_items.clear();
    vector_npcs.clear();
    vector_moving_platforms.clear();
    vector_triggers.clear();
    vector_doors.clear();
    vector_traps.clear();
    vector_signs.clear();
    vector_springs.clear();
    vector_boosters.clear();

    //*******************************************//
    // First, we will read the level properties: //
    //*******************************************//

    //First, we open the file for reading.
    ifstream load;
    load.open("level_properties.blazelevel",ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){

        //Create any variables that will be used to pass on information from the level file.
        double start_y=0;
        double width=0;
        double height=0;
        short number_of_background_layers=0;
        double parallax_speed_x=0;
        double parallax_speed_y=0;

        //The first two numbers in the level file are the x and y dimensions of the entire level's map.
        load >> level_x;
        load >> level_y;

        //The next number is the direction the player should be facing.
        load >> facing;

        load >> start_y;
        load >> width;
        load >> height;

        //Load in the number of background layers.
        load >> number_of_background_layers;

        for(int i=0;i<number_of_background_layers;i++){
            load >> parallax_speed_x;
            load >> parallax_speed_y;
            background_layers.push_back(Background_Layer(start_y,width,height,parallax_speed_x,parallax_speed_y));
        }

        load >> persistent_level_data;
        load >> night;
        load >> deadly_water;

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    else{
        new_map=true;
        new_level(10,10);
    }

    //If the level is loading (and thus a new level was not needed), continue to load the rest of it.
    if(!new_map){

        //*******************************//
        // Next, we will read the tiles: //
        //*******************************//

        //First, we open the file for reading.
        load.open("tiles.blazelevel",ifstream::in);

        if(load!=NULL){

            tile_array.clear();
            tile_array.resize(level_x/32,vector<Tile>(level_y/32));

            //Now we make temporary variables for getting the type of each tile being set.
            int solidity=0;
            int sprite=0;
            int special=0;
            bool foreground=0;
            short angle=0;
            ///short height_mask[TILE_W];

            //Then we iterate through all of the tiles in the file, setting each one.
            //Note that the constant integer '32' here must be changed both here and in the Tile class (where it is used as the tile's width and height).
            for(int y=0;y<level_y/32;y++){
                for(int x=0;x<level_x/32;x++){
                    //For each tile, load the type information from the level file.
                    load >> solidity;
                    load >> sprite;
                    load >> special;
                    load >> foreground;
                    load >> angle;
                    /**for(short i=0;i<TILE_W;i++){
                        load >> height_mask[i];
                    }*/

                    //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
                    tile_array[x][y].x=x*32;
                    tile_array[x][y].y=y*32;
                    tile_array[x][y].solidity=solidity;
                    tile_array[x][y].sprite=sprite;
                    tile_array[x][y].special=special;
                    tile_array[x][y].foreground=foreground;
                    tile_array[x][y].angle=angle;
                    /**for(short i=0;i<TILE_W;i++){
                        tile_array[x][y].height_mask[i]=height_mask[i];
                    }*/
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading terrain tiles.\n");
        }

        //******************************************//
        // Next, we will read the background tiles: //
        //******************************************//

        //First, we open the file for reading.
        load.open("tiles_background.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            tile_background_array.clear();

            tile_background_array.resize(level_x/32,vector<Tile_Background>(level_y/32));

            //Now we make a temporary variable for getting the sprite of each tile being set.
            int sprite=0;

            //Then we iterate through all of the tiles in the file, setting each one.
            //Note that the constant integer '32' here must be changed both here and in the Tile_Background class (where it is used as the tile's width and height).
            for(int int_y=0;int_y<level_y/32;int_y++){
                for(int int_x=0;int_x<level_x/32;int_x++){
                    //For each tile, load the type information from the level file.
                    load >> sprite;

                    //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
                    tile_background_array[int_x][int_y].x=int_x*32;
                    tile_background_array[int_x][int_y].y=int_y*32;
                    tile_background_array[int_x][int_y].sprite=sprite;
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading background tiles.\n");
        }

        //******************************//
        // Now, we will read the items: //
        //******************************//

        //First, we open the file for reading.
        load.open("items.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make a temporary variable for getting the type of each item being loaded.
            int type;
            double x;
            double y;
            int goal_level_to_load;
            bool goal_secret;

            //Then we iterate through all of the items in the file, setting each one.
            while(!load.eof()){
                type=30000;
                x=0;
                y=0;
                goal_level_to_load=0;
                goal_secret=false;

                //For each item, load the type information from the file.
                load >> type;
                load >> x;
                load >> y;
                load >> goal_level_to_load;
                load >> goal_secret;

                //Then, if the type is not 30000, generate a new item in the item vector with the proper coordinates and the type loaded from the file.
                if(type!=30000){
                    vector_items.push_back(Item(x,y,type,goal_level_to_load,goal_secret));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading items.\n");
        }

        //*****************************//
        // Now, we will read the npcs: //
        //*****************************//

        //First, we open the file for reading.
        load.open("npcs.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make a temporary variable for getting the type of each npc being loaded.
            int type;
            double x;
            double y;

            //Then we iterate through all of the npcs in the file, setting each one.
            while(!load.eof()){
                type=0;
                x=0;
                y=0;

                //For each item, load the type information from the file.
                load >> type;
                load >> x;
                load >> y;

                //Then, if the type is nonzero, generate a new item in the item vector with the proper coordinates and the type loaded from the file.
                if(type!=0){
                    vector_npcs.push_back(Npc(x,y,type));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading npcs.\n");
        }

        //*****************************************//
        // Now, we will read the moving platforms: //
        //*****************************************//

        //First, we open the file for reading.
        load.open("moving_platforms.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double move_speed;
            double start_point_x;
            double start_point_y;
            double end_point_x;
            double end_point_y;
            bool active;
            bool round_trip;
            bool movement_type;
            short identifier;
            int number_of_waypoints;
            vector<moving_platform_waypoint> waypoints;

            //Then we iterate through all of the moving platforms in the file, setting each one.
            while(!load.eof()){
                move_speed=0;
                start_point_x=0;
                start_point_y=0;
                end_point_x=0;
                end_point_y=0;
                active=false;
                round_trip=false;
                movement_type=false;
                identifier=0;
                number_of_waypoints=0;
                waypoints.clear();

                //For each item, load the type information from the file.
                load >> move_speed;
                load >> start_point_x;
                load >> start_point_y;
                load >> end_point_x;
                load >> end_point_y;
                load >> active;
                load >> round_trip;
                load >> movement_type;
                load >> identifier;
                load >> number_of_waypoints;

                for(int i=0;i<number_of_waypoints;i++){
                    waypoints.push_back(moving_platform_waypoint());
                    load >> waypoints[i].x;
                    load >> waypoints[i].y;
                }

                if(move_speed!=0){
                    vector_moving_platforms.push_back(Moving_Platform(move_speed,start_point_x,start_point_y,end_point_x,end_point_y,waypoints,active,round_trip,movement_type,identifier));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading moving platforms.\n");
        }

        //*********************************//
        // Now, we will read the triggers: //
        //*********************************//

        //First, we open the file for reading.
        load.open("triggers.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            double w,h;
            int number_of_targets;
            vector<target> targets;
            bool trigger_method;
            short user_type;
            bool repeating;
            short repeat_time;
            short render_trigger;

            //Then we iterate through all of the triggers in the file, setting each one.
            while(!load.eof()){
                x=0;
                y=0;
                w=0;
                h=0;
                number_of_targets=0;
                targets.clear();
                trigger_method=false;
                user_type=0;
                repeating=false;
                repeat_time=0;
                render_trigger=0;

                //For each item, load the type information from the file.
                load >> x;
                load >> y;
                load >> w;
                load >> h;
                load >> number_of_targets;

                for(int i=0;i<number_of_targets;i++){
                    targets.push_back(target());
                    load >> targets[i].type;
                    load >> targets[i].identifier;
                }

                load >> trigger_method;
                load >> user_type;
                load >> repeating;
                load >> repeat_time;
                load >> render_trigger;

                //
                if(w!=0){
                    vector_triggers.push_back(Trigger(x,y,w,h,targets,trigger_method,user_type,repeating,repeat_time,render_trigger));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading triggers.\n");
        }

        //******************************//
        // Now, we will read the doors: //
        //******************************//

        //First, we open the file for reading.
        load.open("doors.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            short type;
            short number;
            bool open;
            short identifier;

            //Then we iterate through all of the doors in the file, setting each one.
            while(!load.eof()){
                x=0;
                y=0;
                type=0;
                number=0;
                open=false;
                identifier=-1;

                //For each one, load the type information from the file.
                load >> x;
                load >> y;
                load >> type;
                load >> number;
                load >> open;
                load >> identifier;

                //
                if(identifier!=-1){
                    vector_doors.push_back(Door(x,y,type,number,open,identifier));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading doors.\n");
        }

        //******************************//
        // Now, we will read the traps: //
        //******************************//

        //First, we open the file for reading.
        load.open("traps.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            short type;
            bool active;
            short identifier;

            //Then we iterate through all of the traps in the file, setting each one.
            while(!load.eof()){
                x=0;
                y=0;
                type=0;
                active=false;
                identifier=-1;

                //For each one, load the type information from the file.
                load >> x;
                load >> y;
                load >> type;
                load >> active;
                load >> identifier;

                //
                if(identifier!=-1){
                    vector_traps.push_back(Trap(x,y,type,active,identifier));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading traps.\n");
        }

        //******************************//
        // Now, we will read the signs: //
        //******************************//

        //First, we open the file for reading.
        load.open("signs.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            string message;
            short sign_type;
            short font_type;

            //Then we iterate through all of the signs in the file, setting each one.
            while(!load.eof()){
                x=-1;
                y=-1;
                message="";
                sign_type=0;
                font_type=0;

                //For each one, load the type information from the file.
                load >> x;
                load >> y;
                load >> message;
                load >> sign_type;
                load >> font_type;

                boost::algorithm::replace_all(message,"<SPACE>"," ");
                boost::algorithm::replace_all(message,"<NEWLINE>","\xA");

                if(x!=-1 && y!=-1){
                    vector_signs.push_back(Sign(x,y,message,sign_type,font_type));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading signs.\n");
        }

        //********************************//
        // Now, we will read the springs: //
        //********************************//

        //First, we open the file for reading.
        load.open("springs.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            double speed;
            short direction;

            //Then we iterate through all of the springs in the file, setting each one.
            while(!load.eof()){
                x=-1;
                y=-1;
                speed=0.0;
                direction=LEFT;

                //For each one, load the type information from the file.
                load >> x;
                load >> y;
                load >> speed;
                load >> direction;

                if(x!=-1 && y!=-1){
                    vector_springs.push_back(Spring(x,y,speed,direction));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading springs.\n");
        }

        //*********************************//
        // Now, we will read the boosters: //
        //*********************************//

        //First, we open the file for reading.
        load.open("boosters.blazelevel",ifstream::in);

        //As long as the file opened successfully, proceed to load the level data.
        if(load!=NULL){
            //Now we make temporary variables.
            double x,y;
            double speed;
            short direction;

            //Then we iterate through all of the boosters in the file, setting each one.
            while(!load.eof()){
                x=-1;
                y=-1;
                speed=0.0;
                direction=LEFT;

                //For each one, load the type information from the file.
                load >> x;
                load >> y;
                load >> speed;
                load >> direction;

                if(x!=-1 && y!=-1){
                    vector_boosters.push_back(Booster(x,y,speed,direction));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            fprintf(stderr,"Error loading boosters.\n");
        }
    }

    //Determine the brush's starting position.
    for(int i=0;i<vector_items.size();i++){
        if(vector_items.at(i).type==4){
            brush.x=vector_items.at(i).x;
            brush.y=vector_items.at(i).y;
            break;
        }
        if(i==vector_items.size()-1){
            brush.x=0;
            brush.y=0;
        }
    }
    for(int int_y=0;int_y<(level_y/32);int_y++){
        for(int int_x=0;int_x<(level_x/32);int_x++){
            if(collision_check(brush.x,brush.y,brush.w,brush.h,tile_array[int_x][int_y].x,tile_array[int_x][int_y].y,TILE_W,TILE_H)){
                brush.x=tile_array[int_x][int_y].x;
                brush.y=tile_array[int_x][int_y].y;
            }
        }
    }
}

void Level::new_level(int get_x,int get_y){
    background_layers.clear();

    vector_items.clear();
    vector_npcs.clear();
    vector_moving_platforms.clear();
    vector_triggers.clear();
    vector_doors.clear();
    vector_traps.clear();
    vector_signs.clear();
    vector_springs.clear();
    vector_boosters.clear();

    tile_array.clear();
    tile_background_array.clear();

    level_x=get_x*32;
    level_y=get_y*32;
    facing=3;

    persistent_level_data=true;
    night=false;
    deadly_water=false;

    int solidity=0;
    int sprite=0;
    int special=0;
    bool foreground=0;
    short angle=0;
    ///short height_mask[32];

    tile_array.resize(level_x/32,vector<Tile>(level_y/32));

    for(int y=0;y<(level_y/32);y++){
        for(int x=0;x<(level_x/32);x++){
            solidity=0;
            sprite=0;
            special=0;
            foreground=0;
            angle=180;
            /**for(short i=0;i<TILE_W;i++){
                height_mask[i]=TILE_H-1;
            }*/

            //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
            tile_array[x][y].x=x*32;
            tile_array[x][y].y=y*32;
            tile_array[x][y].solidity=solidity;
            tile_array[x][y].sprite=sprite;
            tile_array[x][y].special=special;
            tile_array[x][y].foreground=foreground;
            tile_array[x][y].angle=angle;
            /**for(short i=0;i<TILE_W;i++){
                tile_array[x][y].height_mask[i]=height_mask[i];
            }*/
        }
    }

    tile_background_array.resize(level_x/32,vector<Tile_Background>(level_y/32));

    for(int y=0;y<(level_y/32);y++){
        for(int x=0;x<(level_x/32);x++){
            //For each tile, load the type information from the level file.
            sprite=0;

            //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
            tile_background_array[x][y].x=x*32;
            tile_background_array[x][y].y=y*32;
            tile_background_array[x][y].sprite=sprite;
        }
    }

    brush.x=0;
    brush.y=0;
}

void Level::save_level(bool autosave){
    //Now we make temporary variables for getting the type of each tile being set.
    int solidity=0;
    int sprite=0;
    int special=0;
    bool foreground=0;
    short angle=0;
    ///short height_mask[TILE_W];

    //Used to store the filename of the save.
    string save_name;

    //The following two sections of code grab the current system time and store it for use with autosaving.

    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);

    //Store the date and time in buff.
    strftime(buff,sizeof buff,"%Y-%A_%B_%d-%H%M.%S",tm_now);

    //****************************//
    // Save the level properties: //
    //****************************//

    //If autosave==true, then setup the save filename to save to a new file in the autosaves folder. Otherwise, save the file in the standard location.
    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_level_properties.blazelevel";
    }
    else{
        save_name="level_properties.blazelevel";
    }

    ofstream save;

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //The first two numbers in the level file are the x and y dimensions of the entire level's map.
        save << level_x;
        save << " ";
        save << level_y;
        save << " ";

        //The next number is the direction the player should be facing.
        save << facing;
        save << " ";

        //start_y
        if(background_layers.size()!=0){
            save << background_layers[0].start_y;
            save << " ";
        }
        else{
            save << 0;
            save << " ";
        }

        //width
        if(background_layers.size()!=0){
            save << background_layers[0].width;
            save << " ";
        }
        else{
            save << 0;
            save << " ";
        }

        //height
        if(background_layers.size()!=0){
            save << background_layers[0].height;
            save << " ";
        }
        else{
            save << 0;
            save << " ";
        }

        //number_of_background_layers
        save << background_layers.size();
        save << "\n";

        //
        for(int i=0;i<background_layers.size();i++){
            //
            save << background_layers[i].parallax_speed_x;
            save << " ";
            save << background_layers[i].parallax_speed_y;
            save << " ";
        }

        save << "\n";

        save << persistent_level_data;
        save << "\n";

        save << night;
        save << "\n";

        save << deadly_water;
        save << "\n";

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving level properties file.\n");
    }

    //*************************//
    // Save the terrain tiles: //
    //*************************//

    //If autosave==true, then setup the save filename to save to a new file in the autosaves folder. Otherwise, save the file in the standard location.
    if(autosave==true){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_tiles.blazelevel";
    }
    else{
        save_name="tiles.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the tiles in the file, setting each one.
        for(int y=0;y<level_y/32;y++){
            for(int x=0;x<level_x/32;x++){
                solidity=tile_array[x][y].solidity;
                sprite=tile_array[x][y].sprite;
                special=tile_array[x][y].special;
                foreground=tile_array[x][y].foreground;
                angle=tile_array[x][y].angle;
                /**for(short i=0;i<TILE_W;i++){
                    height_mask[i]=tile_array[x][y].height_mask[i];
                }*/

                //For each tile, save the type information from the level file.
                save << solidity;
                save << " ";
                save << sprite;
                save << " ";
                save << special;
                save << " ";
                save << foreground;
                save << " ";
                save << angle;
                save << " ";
                /**for(short i=0;i<TILE_W;i++){
                    save << height_mask[i];
                    save << " ";
                }*/
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving terrain tiles file.\n");
    }

    //****************************//
    // Save the background tiles: //
    //****************************//

    //If autosave==true, then setup the save filename to save to a new file in the autosaves folder. Otherwise, save the file in the standard location.
    if(autosave==true){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_tiles_background.blazelevel";
    }
    else{
        save_name="tiles_background.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //We iterate through all of the tiles in the file, setting each one.
        for(int y=0;y<level_y/32;y++){
            for(int x=0;x<level_x/32;x++){
                sprite=tile_background_array[x][y].sprite;
                //For each tile, save the type information from the level in memory.
                save << sprite;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving background tiles file.\n");
    }

    //*****************//
    // Save the items: //
    //*****************//

    if(autosave==true){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_items.blazelevel";
    }
    else{
        save_name="items.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the items in the file, setting each one.
        for(int i=0;i<vector_items.size();i++){
            //For each item, save the type information from the level file.
            save << vector_items.at(i).type;
            save << " ";
            save << vector_items.at(i).x;
            save << " ";
            save << vector_items.at(i).y;
            save << " ";
            save << vector_items.at(i).goal_level_to_load;
            save << " ";
            save << vector_items.at(i).goal_secret;
            save << " ";
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving items file.\n");
    }

    //****************//
    // Save the npcs: //
    //****************//

    if(autosave==true){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_npcs.blazelevel";
    }
    else{
        save_name="npcs.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the npcs in the file, setting each one.
        for(int i=0;i<vector_npcs.size();i++){
            //As long as the npc has a type.
            if(vector_npcs.at(i).type!=0){
                //For each npc, save the type information from the level file.
                save << vector_npcs.at(i).type;
                save << " ";
                save << vector_npcs.at(i).x;
                save << " ";
                save << vector_npcs.at(i).y;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving npcs file.\n");
    }

    //****************************//
    // Save the moving platforms: //
    //****************************//

    if(autosave==true){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_moving_platforms.blazelevel";
    }
    else{
        save_name="moving_platforms.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the moving platforms in the file, setting each one.
        for(int i=0;i<vector_moving_platforms.size();i++){
            //As long as the platform's move_speed is not 0.
            if(vector_moving_platforms.at(i).move_speed!=0){
                //For each npc, save the type information from the level file.
                save << vector_moving_platforms.at(i).move_speed;
                save << " ";
                save << vector_moving_platforms.at(i).start_point_x;
                save << " ";
                save << vector_moving_platforms.at(i).start_point_y;
                save << " ";
                save << vector_moving_platforms.at(i).end_point_x;
                save << " ";
                save << vector_moving_platforms.at(i).end_point_y;
                save << " ";
                save << vector_moving_platforms.at(i).active;
                save << " ";
                save << vector_moving_platforms.at(i).round_trip;
                save << " ";
                save << vector_moving_platforms.at(i).movement_type;
                save << " ";
                save << vector_moving_platforms.at(i).identifier;
                save << " ";

                save << vector_moving_platforms.at(i).waypoints.size();
                save << " ";
                for(int n=0;n<vector_moving_platforms[i].waypoints.size();n++){
                    save << vector_moving_platforms[i].waypoints[n].x;
                    save << " ";
                    save << vector_moving_platforms[i].waypoints[n].y;
                    save << " ";
                }
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving moving platforms file.\n");
    }

    //********************//
    // Save the triggers: //
    //********************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_triggers.blazelevel";
    }
    else{
        save_name="triggers.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the triggers in the file, setting each one.
        for(int i=0;i<vector_triggers.size();i++){
            //As long as the trigger's width is not 0.
            if(vector_triggers.at(i).w!=0){
                //For each npc, save the type information from the level file.
                save << vector_triggers[i].x;
                save << " ";
                save << vector_triggers[i].y;
                save << " ";
                save << vector_triggers[i].w;
                save << " ";
                save << vector_triggers[i].h;
                save << " ";

                save << vector_triggers[i].targets.size();
                save << " ";
                for(int n=0;n<vector_triggers[i].targets.size();n++){
                    save << vector_triggers[i].targets[n].type;
                    save << " ";
                    save << vector_triggers[i].targets[n].identifier;
                    save << " ";
                }

                save << vector_triggers[i].trigger_method;
                save << " ";
                save << vector_triggers[i].user_type;
                save << " ";
                save << vector_triggers[i].repeating;
                save << " ";
                save << vector_triggers[i].repeat_time;
                save << " ";
                save << vector_triggers[i].render_trigger;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving triggers file.\n");
    }

    //*****************//
    // Save the doors: //
    //*****************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_doors.blazelevel";
    }
    else{
        save_name="doors.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the doors in the file, setting each one.
        for(int i=0;i<vector_doors.size();i++){
            //As long as the door's identifier is not -1.
            if(vector_doors.at(i).identifier!=-1){
                save << vector_doors[i].x;
                save << " ";
                save << vector_doors[i].y;
                save << " ";
                save << vector_doors[i].type;
                save << " ";
                save << vector_doors[i].number;
                save << " ";
                save << vector_doors[i].open;
                save << " ";
                save << vector_doors[i].identifier;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving doors file.\n");
    }

    //*****************//
    // Save the traps: //
    //*****************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_traps.blazelevel";
    }
    else{
        save_name="traps.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the traps in the file, setting each one.
        for(int i=0;i<vector_traps.size();i++){
            //As long as the trap's identifier is not -1.
            if(vector_traps.at(i).identifier!=-1){
                save << vector_traps[i].x;
                save << " ";
                save << vector_traps[i].y;
                save << " ";
                save << vector_traps[i].type;
                save << " ";
                save << vector_traps[i].active;
                save << " ";
                save << vector_traps[i].identifier;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving traps file.\n");
    }

    //*****************//
    // Save the signs: //
    //*****************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_signs.blazelevel";
    }
    else{
        save_name="signs.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the signs in the file, setting each one.
        for(int i=0;i<vector_signs.size();i++){
            if(vector_signs[i].x!=-1 && vector_signs[i].y!=-1){
                boost::algorithm::replace_all(vector_signs[i].message," ","<SPACE>");
                boost::algorithm::replace_all(vector_signs[i].message,"\xA","<NEWLINE>");

                save << vector_signs[i].x;
                save << " ";
                save << vector_signs[i].y;
                save << " ";
                save << vector_signs[i].message;
                save << " ";
                save << vector_signs[i].type;
                save << " ";
                save << vector_signs[i].font_type;
                save << " ";

                boost::algorithm::replace_all(vector_signs[i].message,"<SPACE>"," ");
                boost::algorithm::replace_all(vector_signs[i].message,"<NEWLINE>","\xA");
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving signs file.\n");
    }

    //*******************//
    // Save the springs: //
    //*******************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_springs.blazelevel";
    }
    else{
        save_name="springs.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the springs in the file, setting each one.
        for(int i=0;i<vector_springs.size();i++){
            if(vector_springs[i].x!=-1 && vector_springs[i].y!=-1){
                save << vector_springs[i].x;
                save << " ";
                save << vector_springs[i].y;
                save << " ";
                save << vector_springs[i].speed;
                save << " ";
                save << vector_springs[i].direction;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving springs file.\n");
    }

    //********************//
    // Save the boosters: //
    //********************//

    if(autosave){
        //Set the filename.
        save_name="autosaves/";
        save_name+=buff;
        save_name+="_boosters.blazelevel";
    }
    else{
        save_name="boosters.blazelevel";
    }

    save.open(save_name.c_str());

    //As long as the file opened successfully, proceed to load the level data.
    if(save!=NULL){
        //Then we iterate through all of the boosters in the file, setting each one.
        for(int i=0;i<vector_boosters.size();i++){
            if(vector_boosters[i].x!=-1 && vector_boosters[i].y!=-1){
                save << vector_boosters[i].x;
                save << " ";
                save << vector_boosters[i].y;
                save << " ";
                save << vector_boosters[i].speed;
                save << " ";
                save << vector_boosters[i].direction;
                save << " ";
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving boosters file.\n");
    }
}

void Level::resize_level(int get_x,int get_y){
    //If either new dimension is smaller than the old, abort.
    if(get_x<level_x/32 || get_y<level_y/32){
        return;
    }
    //Likewise, if both dimensions are the same as before, abort.
    else if(get_x==level_x/32 && get_y==level_y/32){
        return;
    }

    int old_level_x=level_x;
    int old_level_y=level_y;

    level_x=get_x*32;
    level_y=get_y*32;

    vector< vector<Tile> > tile_array_backup=tile_array;

    tile_array.clear();
    tile_array.resize(level_x/32,vector<Tile>(level_y/32));

    for(int x=0;x<old_level_x/32;x++){
        for(int y=0;y<old_level_y/32;y++){
            tile_array[x][y]=tile_array_backup[x][y];
        }
    }

    for(int x=0;x<level_x/32;x++){
        for(int y=0;y<level_y/32;y++){
            tile_array[x][y].x=x*32;
            tile_array[x][y].y=y*32;
        }
    }

    vector< vector<Tile_Background> > tile_background_array_backup=tile_background_array;

    tile_background_array.clear();
    tile_background_array.resize(level_x/32,vector<Tile_Background>(level_y/32));

    for(int x=0;x<old_level_x/32;x++){
        for(int y=0;y<old_level_y/32;y++){
            tile_background_array[x][y]=tile_background_array_backup[x][y];
        }
    }

    for(int x=0;x<level_x/32;x++){
        for(int y=0;y<level_y/32;y++){
            tile_background_array[x][y].x=x*32;
            tile_background_array[x][y].y=y*32;
        }
    }
}

void Level::new_random_level(int get_x,int get_y){
    new_level(get_x,get_y);

    //The whole level is rock.
    for(int x=0;x<get_x;x++){
        for(int y=0;y<get_y;y++){
            tile_array[x][y].solidity=TILE_SOLIDITY_SOLID;
            tile_array[x][y].sprite=10;
            tile_array[x][y].special=TILE_SPECIAL_NONE;
            tile_array[x][y].foreground=false;
            tile_background_array[x][y].sprite=0;
        }
    }

    int number_of_rooms=(int)floor((double)(get_x*get_y)/4000.0);
    vector<Room> rooms;
    for(int i=0;i<number_of_rooms;i++){
        rooms.push_back(Room());
        rooms[rooms.size()-1].x=random_range(0,get_x-1);
        rooms[rooms.size()-1].y=random_range(48,get_y-1);
        if(random_range(0,99)<75){
            rooms[rooms.size()-1].room_size=random_range(5,8);
        }
        else{
            rooms[rooms.size()-1].room_size=random_range(8,16);
        }

        if(room_overlaps_room(rooms,rooms.size()-1)){
            rooms.pop_back();
        }
        else{
            for(int x=rooms[rooms.size()-1].x-rooms[rooms.size()-1].room_size;x<=rooms[rooms.size()-1].x+rooms[rooms.size()-1].room_size;x++){
                for(int y=rooms[rooms.size()-1].y-rooms[rooms.size()-1].room_size;y<=rooms[rooms.size()-1].y+rooms[rooms.size()-1].room_size;y++){
                    if(x>=0 && x<=get_x-1 && y>=0 && y<=get_y-1){
                        tile_array[x][y].solidity=TILE_SOLIDITY_PASSABLE;
                        tile_array[x][y].sprite=0;
                        tile_array[x][y].special=TILE_SPECIAL_NONE;
                        tile_array[x][y].foreground=false;
                        tile_background_array[x][y].sprite=1;
                    }
                }
            }
        }
    }

    for(int i=0;i<rooms.size();i++){
        for(int n=i+1;n<rooms.size();n++){
            draw_line(rooms[i].x,rooms[i].y,rooms[n].x,rooms[n].y);
        }
    }

    //Above ground is air.
    for(int x=0;x<get_x;x++){
        for(int y=0;y<48-25;y++){
            if(x>=0 && x<=get_x-1 && y>=0 && y<=get_y-1){
                tile_array[x][y].solidity=TILE_SOLIDITY_PASSABLE;
                tile_array[x][y].sprite=0;
                tile_array[x][y].special=TILE_SPECIAL_NONE;
                tile_array[x][y].foreground=false;
                tile_background_array[x][y].sprite=0;
            }
        }
    }

    //Create the entrance.
    int entrance_x=random_range(25,get_x-1-25);
    int entrance_y=48-25-1;

    vector_items.push_back(Item(entrance_x*TILE_W,entrance_y*TILE_H,ITEM_SPAWNPOINT,0,false));

    tile_array[entrance_x][entrance_y].solidity=TILE_SOLIDITY_PASSABLE;
    tile_array[entrance_x][entrance_y].sprite=32;
    tile_array[entrance_x][entrance_y].special=TILE_SPECIAL_NONE;
    tile_array[entrance_x][entrance_y].foreground=true;
    tile_background_array[entrance_x][entrance_y].sprite=0;

    tile_array[entrance_x][entrance_y+1].solidity=TILE_SOLIDITY_CLOUD;
    tile_array[entrance_x][entrance_y+1].sprite=31;
    tile_array[entrance_x][entrance_y+1].special=TILE_SPECIAL_NONE;
    tile_array[entrance_x][entrance_y+1].foreground=true;
    tile_background_array[entrance_x][entrance_y+1].sprite=1;

    for(int y=48-25;true;){
        y++;

        int x=entrance_x;

        if(x>=0 && x<=get_x-1 && y>=0 && y<=get_y-1){
            if(tile_array[x][y].solidity!=TILE_SOLIDITY_PASSABLE){
                tile_array[x][y].solidity=TILE_SOLIDITY_PASSABLE;
                tile_array[x][y].sprite=0;
                tile_array[x][y].special=TILE_SPECIAL_NONE;
                tile_array[x][y].foreground=false;
                tile_background_array[x][y].sprite=1;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
}

bool Level::room_overlaps_room(vector<Room> rooms,int room_index){
    for(int i=0;i<rooms.size();i++){
        if(i!=room_index){
            if(collision_check((rooms[i].x-rooms[i].room_size)*TILE_W,(rooms[i].y-rooms[i].room_size)*TILE_H,(1+rooms[i].room_size*2)*TILE_W,(1+rooms[i].room_size*2)*TILE_H,(rooms[room_index].x-rooms[room_index].room_size)*TILE_W,(rooms[room_index].y-rooms[room_index].room_size)*TILE_H,(1+rooms[room_index].room_size*2)*TILE_W,(1+rooms[room_index].room_size*2)*TILE_H)){
                return true;
            }
        }
    }

    return false;
}

void Level::draw_line(int start_x,int start_y,int end_x,int end_y){
    int get_x=level_x/32;
    int get_y=level_y/32;

    while(start_x!=end_x || start_y!=end_y){
        for(int x=start_x-1;x<=start_x+1;x++){
            for(int y=start_y-1;y<=start_y+1;y++){
                if(x>=0 && x<=get_x-1 && y>=0 && y<=get_y-1){
                    tile_array[x][y].solidity=TILE_SOLIDITY_PASSABLE;
                    tile_array[x][y].sprite=0;
                    tile_array[x][y].special=TILE_SPECIAL_NONE;
                    tile_array[x][y].foreground=false;
                    tile_background_array[x][y].sprite=1;
                }
            }
        }

        if(start_x!=end_x){
            if(start_x<end_x){
                start_x++;
            }
            else{
                start_x--;
            }
        }

        if(start_y!=end_y){
            if(start_y<end_y){
                start_y++;
            }
            else{
                start_y--;
            }
        }

        if(random_range(0,99)<1){
            break;
        }
    }
}
