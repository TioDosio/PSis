#include "server-func.h"
#include "../common-files/lizardsNroaches.h"
#include <time.h>
#include "../common-files/display-funcs.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
/*
 * @brief clear responde parameters
 *
 * @param r response message to be cleared
 * @param suc success parameter
 * @param code secret code parameter
 * @param score score parameter
 *
 */
void generate_r(response_msg *r, int suc, int code, int score)
{
    r->success = suc;
    r->secret_code = code;
    r->score = score;
}

/*
 * @brief generate random character
 *
 * @return random character
 *
 */
char generateRandomChar()
{
    static char usedChars[26] = {0}; // Keeps track of used characters
    int randomIndex;

    // Seed the random number generator
    srand(time(NULL));

    // Loop until a unique character is found
    do
    {
        randomIndex = rand() % 26; // Generate a random index between 0 and 25
    } while (usedChars[randomIndex] == 1);

    // Mark the character as used
    usedChars[randomIndex] = 1;

    // Convert the index to the corresponding character
    char randomChar = 'a' + randomIndex;

    return randomChar;
}

/*
 * @brief generate random code
 *
 * @param lizard_array array of lizards
 * @param roach_array array of roaches
 * @param n_lizards number of lizards
 * @param n_roaches number of roaches
 *
 * @return random code
 *
 */
int generate_code(entity_t lizard_array[], entity_t roach_array[], int n_lizards, int n_roaches)
{
    while (1)
    {
        int found = 0;
        short int code = rand() % 30000;

        // Check if code is already in use (lizards)
        for (int i = 0; i < n_lizards; i++)
        {
            if (lizard_array[i].secret_code == code)
            {
                found = 1;
                break;
            }
        }
        // If code is already in use, generate another one
        if (found)
        {
            continue;
        }
        // Check if code is already in use (roaches)
        for (int i = 0; i < n_roaches; i++)
        {
            if (roach_array[i].secret_code == code)
            {
                found = 1;
                break;
            }
        }
        // If code is not in use, return it
        if (!found)
        {
            return code;
        }
    }
}

// Returns array pos if it is on the given position
// Returns -1 if there is no entity on the given position

/*
 * @brief  check if there is an entity on the given position
 *
 * @param entity[] array of entities
 * @param n_entities number of entities
 * @param x x position
 * @param y y position
 *
 * @return array pos if it is on the given position
 *
 */
int on_pos(entity_t entity[], int n_entities, int x, int y)
{
    // Seach for entity on the given position
    for (int i = 0; i < n_entities; i++)
    {
        if (entity[i].pos_x == x && entity[i].pos_y == y)
        {
            return i;
        }
    }
    return -1;
}

/*
 * @brief  set new position based on direction
 *
 * @param x x position
 * @param y y position
 * @param direction direction of movement
 *
 */
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

entity_t *move_entity(generic_msg m, response_msg *r, entity_t array_entity[], entity_t lizard_array[], int n_entity, int n_lizards, int *entity_id, int *id_bumped)
{
    entity_t old_entity; // aux variable to store current values
    entity_t new_entity; // aux variable to store new values
    int code = m.secret_code;
    int pos_x;
    int pos_y;

    *entity_id = find_entity_id(array_entity, n_entity, code);

    if (*entity_id == -1) // if entity not found
    {
        generate_r(r, 0, code, 0);
        return NULL;
    }

    old_entity = array_entity[*entity_id]; // save old values

    r->secret_code = old_entity.secret_code;

    // Copy old values to new entity
    new_entity = old_entity;

    // update new pos from direction message
    pos_x = old_entity.pos_x;
    pos_y = old_entity.pos_y;
    new_entity.direction = m.direction;
    new_position(&pos_x, &pos_y, m.direction);

    // Check if there is another lizard on the new position and undo movement if so
    *id_bumped = on_pos(lizard_array, n_lizards, pos_x, pos_y);
    if (*id_bumped != -1)
    {
        // If there is a snake on the new position, do not move
        pos_x = old_entity.pos_x;
        pos_y = old_entity.pos_y;
        // If moving a lizard, average points of both
        if (new_entity.entity_type == LIZARD)
        {
            new_entity.points = (old_entity.points + array_entity[*id_bumped].points) / 2;
            array_entity[*id_bumped].points = new_entity.points;
        }
    }

    // Finish movement and update data
    new_entity.pos_x = pos_x;
    new_entity.pos_y = pos_y;

    array_entity[*entity_id] = new_entity;

    return &array_entity[*entity_id];
}

void eat_roaches(entity_t *lizard, entity_t roach_array[], int n, time_t roach_death_time[])
{
    int i = 0;
    int x = lizard->pos_x;
    int y = lizard->pos_y;
    for (i = 0; i < n; i++)
    {
        if (roach_array[i].pos_x == x && roach_array[i].pos_y == y)
        {
            if (roach_array[i].ch != ' ')
                lizard->points += roach_array[i].ch - '0'; // if roach exists, gain points
            // turn roach invisible and pointless for five seconds
            roach_array[i].ch = ' ';
            roach_death_time[i] = time(NULL);
        }
    }
}

void respawn_roach(entity_t *roach)
{
    roach->ch = roach->points + '0';
    roach->pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
    roach->pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
}