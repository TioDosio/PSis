#include "../common-files/lizardsNroachesNwasps.h"


/**
 * @brief Function to move lizard with given code in given direction
 * 
 * @param code code of entity
 * @param dir direction to move entity
 * @param game struct containing all game data
 * 
 * @return 0 if failed
 * 
 */
int move_lizard (int code, direction_t dir , thread_args *game);

/**
 * @brief Analize if position is valid for movement. If not, save type of collision and 
 * index of entity collided with onto collision_type and collision_index pointers.
 * 
 * @param game struct containing all game data
 * @param pos_x x position to check
 * @param pos_y y position to check
 * @param collision_index pointer to index of entity collided with
 * @param collision_type pointer to type of entity collided with
 * 
 * @return 1 if valid position, 0 if not
 * 
 */
int valid_pos(thread_args *game,int pos_x,int pos_y, int *collision_index, entity_t *collision_type);

/**
 * @brief Eats roaches in given position and returns points gained
 * @param game struct containing all game data
 * @param pos_x x position to check
 * @param pos_y y position to check
 * 
 * @return number of points gained
 * 
 */
int eat_roaches(thread_args *game, int pos_x, int pos_y);