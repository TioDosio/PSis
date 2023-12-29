#include "entity_behaviour.h"
#include "thread-funcs.h"

int move_lizard(int code, direction_t dir , thread_args *game)
{
    entity_t lizard_after_move;
    int lizard_index;

    int pos_x = -1;
    int pos_y = -1;
    
    //Critial section - Lizard mutex always locked first
    pthread_mutex_lock(&mutex_lizard);
    pthread_mutex_lock(&mutex_npc);
    int success = (lizard_index = find_entity_index(game->lizard_array, game->n_lizards, code) != -1);

    if(success)
    {
        // Save old values
        lizard_after_move = game->lizard_array[lizard_index];
        pos_x = game->lizard_array[lizard_index].pos_x;
        pos_y = game->lizard_array[lizard_index].pos_y;

        // Find new position
        new_position(&pos_x, &pos_y, dir);

        // Check if collision with NPC or another lizard
        int collision_index = -1;
        entity_t collision_type; 

        //If movement is valid
        if (on_pos(game, pos_x, pos_y, &collision_index, &collision_type))
        {
            //Update new position and direction
            lizard_after_move.pos_x = pos_x;
            lizard_after_move.pos_y = pos_y;
            lizard_after_move.direction = dir;

            //eat roaches, if any
            lizard_after_move.points += eat_roaches(game, pos_x, pos_y);
        } else {
            // Handle collision and do not move
            switch (collision_type.entity_type)
            {
                case LIZARD:
                    // If lizard, average points for both Lizards
                    int points = (lizard_after_move.points + collision_type.points) / 2;
                    lizard_after_move.points = points;
                    game->lizard_array[collision_index].points = points;
                    break;
                case WASP:
                    // If wasp, reduce 10 points
                    lizard_after_move.points -= 10;
                    break;
            }
        }

        // Update lizard array
        game->lizard_array[lizard_index] = lizard_after_move;

    }
    
    pthread_mutex_unlock(&mutex_lizard);
    pthread_mutex_unlock(&mutex_npc);

    return success;
}


int valid_pos(thread_args *game,int pos_x,int pos_y, int *collision_index, entity_t *collision_type)
{
    //TO DO

    return 0;
}

int eat_roaches(thread_args *game, int pos_x, int pos_y)
{
    int points = 0;

    //TO DO

    return points;
}