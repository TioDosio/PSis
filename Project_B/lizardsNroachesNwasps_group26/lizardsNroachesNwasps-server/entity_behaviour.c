#include "entity_behaviour.h"
#include "thread-funcs.h"
#include <stdlib.h>

int move_lizard(int code, direction_t dir , thread_args *game)
{
    entity_t lizard_after_move;
    int lizard_index;

    int pos_x = -1;
    int pos_y = -1;
    
    //Critial section - Lizard mutex always locked first
    pthread_mutex_lock(&mutex_lizard);
    pthread_mutex_lock(&mutex_npc);
    
    int success = (lizard_index = find_entity_index(game->lizard_array, game->n_lizards, code)) != -1;

    if(success)
    {
        int points;
        
        // Save old values
        lizard_after_move = game->lizard_array[lizard_index];
        pos_x = game->lizard_array[lizard_index].pos_x;
        pos_y = game->lizard_array[lizard_index].pos_y;

        // Find new position
        new_position(&pos_x, &pos_y, dir);

        // Check if collision with NPC or another lizard
        int collision_index = -1;
        entity_type_t collision_type; 

        //If movement is valid
        if (valid_pos(game, pos_x, pos_y, &collision_index, &collision_type))
        {
            //Update new position and direction
            lizard_after_move.pos_x = pos_x;
            lizard_after_move.pos_y = pos_y;
            lizard_after_move.direction = dir;

            //eat roaches, if any
            lizard_after_move.points += eat_roaches(game, pos_x, pos_y);
        } else {
            // Handle collision and do not move
            switch (collision_type)
            {
                case LIZARD:
                    // If lizard, average points for both Lizards
                    points = (lizard_after_move.points + game->lizard_array[collision_index].points) / 2;
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

int move_npc(int code, direction_t dir , thread_args *game){

    entity_t npc_after_move;
    int npc_index;
    entity_type_t type_of_npc;

    int pos_x = -1;
    int pos_y = -1;
    
    //Critial section - Lizard mutex always locked first
    pthread_mutex_lock(&mutex_lizard);
    pthread_mutex_lock(&mutex_npc);
    
    int success = (npc_index = find_entity_index(game->npc_array, game->n_npc, code)) != -1;

    if(success)
    {
        // Save old values
        npc_after_move = game->npc_array[npc_index];
        pos_x = game->npc_array[npc_index].pos_x;
        pos_y = game->npc_array[npc_index].pos_y;
        type_of_npc = game->npc_array[npc_index].entity_type;

        // Find new position
        new_position(&pos_x, &pos_y, dir);

        // Check if collision with Lizard or another NPC
        int collision_index = -1;
        entity_type_t collision_type; 

        //If movement is valid
        if (valid_pos(game, pos_x, pos_y, &collision_index, &collision_type))
        {
            //Update new position and direction
            npc_after_move.pos_x = pos_x;
            npc_after_move.pos_y = pos_y;
            npc_after_move.direction = dir;
        } else {
            // Handle collision and do not move
            if(type_of_npc == WASP) //Only wasps can collide with other entities
                switch (collision_type)
                {
                    case LIZARD:
                        // If lizard, remove 10 points from lizard
                        game->lizard_array[collision_index].points -= 10;
                        break;
                    case ROACH:
                    case WASP:
                        // If wasp or Roach, do nothing. 
                        break;
                }
        }

        // Update NPC array
        game->npc_array[npc_index] = npc_after_move;
    }
    
    pthread_mutex_unlock(&mutex_lizard);
    pthread_mutex_unlock(&mutex_npc);
    

    return success;
}



int valid_pos(thread_args *game,int pos_x,int pos_y, int *collision_index, entity_type_t *collision_type)
{
    //First, check if Lizard is on the position
    *collision_index = collision_check(game->lizard_array, game->n_lizards, pos_x, pos_y);
    if (*collision_index != -1)
    {
        *collision_type = LIZARD;

        return 0;
    }

    //Then, check if WASP is on the position
    *collision_index = collision_check(game->npc_array, game->n_npc, pos_x, pos_y);
    if (*collision_index != -1)
    {
        *collision_type = WASP;

        return 0;
    }

    //If no collision, return 1, since movement is allowed.

    return 1;
}

int collision_check(entity_t * entity_array, int n_entities, int x, int y)
{
    // Seach for entity on the given position
    for (int i = 0; i < n_entities; i++)
    {
        //Ignore Roaches
        if (entity_array[i].pos_x == x && entity_array[i].pos_y == y && entity_array[i].entity_type != ROACH)
        {
            return i;
        }
    }
    return -1;
}

int eat_roaches(thread_args *game, int pos_x, int pos_y)
{
    int points = 0;
    int i = 0;
    for (i = 0; i < game->n_npc; i++)
    {
        //If Roach is on the position
        if (game->npc_array[i].pos_x == pos_x && game->npc_array[i].pos_y == pos_y && game->npc_array->entity_type == ROACH)
        {
            //If not dead
            if(game->npc_array->ch != ' ')
            {
                //Add points
                points += game->npc_array->ch - '0';
                //Kill Roach
                game->npc_array[i].ch = ' ';
                game->roach_death_time[i] = time(NULL);
            }
            
        }
    }
    return points;
}

int find_entity_index(entity_t *entity_array, int n_entities, int code)
{
    // Seach for entity with the given code
    for (int i = 0; i < n_entities; i++)
    {
        if (entity_array[i].secret_code == code)
        {
            return i;
        }
    }
    return -1;
}

void new_position(int *x, int *y, direction_t direction)
{
    switch (direction)
    {
    case UP:
        (*x)--;
        if (*x == 0)
            *x = 1;
        break;
    case DOWN:
        (*x)++;
        if (*x == WINDOW_SIZE - 1)
            *x = WINDOW_SIZE - 2;
        break;
    case LEFT:
        (*y)--;
        if (*y == 0)
            *y = 1;
        break;
    case RIGHT:
        (*y)++;
        if (*y == WINDOW_SIZE - 1)
            *y = WINDOW_SIZE - 2;
        break;
    default:
        break;
    }
}

int spawn_entity(thread_args *game, entity_type_t entity_type)
{
    entity_t new_entity;
    int pos_x, pos_y;
    int collision_index;            //not used, but needed for valid_pos
    entity_type_t collision_type;   //not used, but needed for valid_pos
    int code;                       // secret code to return

    // Generate random position
    pos_x = rand() % (WINDOW_SIZE - 2) + 1;
    pos_y = rand() % (WINDOW_SIZE - 2) + 1;

    // Check if position is valid
    if (valid_pos(game, pos_x, pos_y, &collision_index, &collision_type))
    {
        // Set entity values
        code = generate_code();
        new_entity.entity_type = entity_type;
        new_entity.points = 0;
        new_entity.pos_x = pos_x;
        new_entity.pos_y = pos_y;
        new_entity.direction = UP;
        new_entity.secret_code = code;

        // Add entity to array
        if (entity_type == LIZARD)
        {
            new_entity.ch = 'a'+game->n_lizards;
            game->lizard_array[game->n_lizards] = new_entity;
            game->n_lizards++;
        }
        else 
        {
            if(entity_type == ROACH)
                new_entity.ch = '0' + rand() % 5;
            else
                new_entity.ch = '#';
            game->npc_array[game->n_npc] = new_entity;
            game->n_npc++;
        }
    } else {
        // If position is not valid, try again
        code = spawn_entity(game, entity_type);
    }

    return new_entity.secret_code;
}