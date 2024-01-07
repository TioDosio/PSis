#include "../common-files/lizardsNroachesNwasps.h"

/**
 * Spawns an entity in the game.
 *
 * This function is responsible for spawning a new entity of the specified type in the game.
 *
 * @param game The game structure containing the necessary information.
 * @param entity_type The type of entity to be spawned.
 * @return An integer indicating the unique code of the spawned entity, or -1 if the entity could not be spawned.
 * @note Must be called with the respective entity mutex locked.
 */
int spawn_entity(thread_args *game, entity_type_t entity_type);

/**
 * @brief Function to move lizard with given code in given direction
 * 
 * @param code code of entity
 * @param dir direction to move entity
 * @param game struct containing all game data
 * @param points pointer to store points gained by lizard. Ignored if NULL
 * @param id_l_bumped pointer to store id of lizard bumped. Ignored if NULL 
 * @param id_l pointer to store id of lizard moved. Ignored if NULL
 * 
 * @return 0 if failed
 * 
 */
int move_lizard (int code, direction_t dir , thread_args *game, int *points, int *id_l_bumped, int *id_l);


/**
 * @brief Moves the non-player character (NPC) in the specified direction.
 *
 * This function is responsible for moving the NPC in the specified direction
 * based on the given code. The NPC's movement is performed within the context
 * of the provided game thread arguments.
 *
 * @param code The code representing the NPC.
 * @param dir The direction in which the NPC should move.
 * @param game The thread arguments containing the game context.
 * @return An integer indicating the success or failure of the NPC's movement.
 */
int move_npc(int code, direction_t dir , thread_args *game, int* id_npc);

/**
 * @note FUNCTION MUST BE CALLED WITH LOCKED MUTEXES.
 * @brief Analizes if position is valid for movement (if there is no Liz or Wasp). 
 * If not, save type of collision and index of entity collided with 
 * onto collision_type and collision_index pointers. 
 * 
 * @param game struct containing all game data
 * @param pos_x x position to check
 * @param pos_y y position to check
 * @param collision_index pointer to index of entity collided with
 * @param collision_type pointer to type of entity collided with
 * 
 * @return 1 if valid position, 0 if not
 *  
 * 
 */
int valid_pos(thread_args *game,int pos_x,int pos_y, int *collision_index, entity_type_t *collision_type);

/**
 * @note FUNCTION MUST BE CALLED WITH LOCKED MUTEXES.
 * @brief Eats roaches in given position and returns points gained
 * @param game struct containing all game data
 * @param pos_x x position to check
 * @param pos_y y position to check
 * 
 * @return number of points gained
 * 
 */
int eat_roaches(thread_args *game, int pos_x, int pos_y);


/**
 * @note FUNCTION MUST BE CALLED WITH LOCKED MUTEXES.
 * @brief Checks if there is a WASP or a LIZARD in given position and returns index of entity
 * @param entity_array array of entities to check
 * @param n_entities number of entities in array
 * @param x x position to check
 * @param y y position to check
 * 
 * @return Index of entity if there is one, -1 if not
 * 
 */
int collision_check(entity_t * entity_array, int n_entities, int x, int y);

/**
 * @brief Finds the index of the specified entity in the entity list.
 *
 * @param entity_array The array of entities to search.
 * @param n_entities The number of entities in the array.
 * @param code The code of the entity to find the index of.
 * @return The index of the entity in the entity list, or -1 if the entity is not found.
 */
int find_entity_index(entity_t *entity_array, int n_entities, int code);


/**
 * @brief Updates the position of an entity based on a given direction.
 *
 * This function updates the position of an entity by modifying the values of the x and y coordinates.
 *
 * @param x Pointer to the current x coordinate of the entity.
 * @param y Pointer to the current y coordinate of the entity.
 * @param direction The direction in which the entity should move.
 */
void new_position(int *x, int *y, direction_t direction);