#ifndef display_funcs_H // Include guards to prevent multiple inclusions
#define display_funcs_H

#include "../common-files/lizardsNroachesNwasps.h"
#include <ncurses.h>


/*
 * @brief clear entity from display
 *
 * @param win window to be cleared
 * @param entity entity to be cleared
 *
 */
void disp_clear_entity(WINDOW *win, entity_t entity);

/*
 * @brief draw entity on display
 *
 * @param win window to be drawn on
 * @param entity entity to be drawn
 *
 */
void disp_draw_entity(WINDOW *win, entity_t entity);

/*
 * @brief clear lizard body
 *
 * @param win window to be cleared
 * @param pos_x x position of the lizard
 * @param pos_y y position of the lizard
 * @param direction direction of the lizard
 *
 */
void clear_body(WINDOW *win, int pos_x, int pos_y, direction_t direction);

/*
 * @brief draw lizard body
 *
 * @param win window to be drawn on
 * @param lizard lizard to be drawn
 *
 */
void draw_body(WINDOW *win, entity_t lizard);

/*
* @brief clear window
*
* @param win window to be cleared
*
*/
void disp_clear_window(WINDOW *win);

/*
 * @brief entity id in array
 *
 * @param entity[] array of entities
 * @param n_entities number of entities
 * @param code secret code of the entity to be found
 *
 *
 */
int find_entity_id(entity_t entity[], int n_entities, int code);

/*
* @brief remove entity from array

* @param entity[] array of entities
* @param n_entities number of entities
* @param id id of the entity to be removed

*/
void remove_entity(entity_t entity[], int *n_entities, int id);

/*
* @brief Updates display

* @param game thread arguments
    
*/
void update_display(thread_args *game);

#endif // display_funcs_H