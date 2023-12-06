#ifndef display_funcs_H  // Include guards to prevent multiple inclusions
#define display_funcs_H

#include "lizardsNroaches.h"
#include <ncurses.h>

// Function to clear entity from display
void disp_clear_entity(WINDOW *win, entity_t entity);

// Function to draw entity on display
void disp_draw_entity(WINDOW *win, entity_t entity);

// Function to clear body of lizards
void clear_body(WINDOW *win, int pos_x, int pos_y, direction_t direction);

// Function to draw body of lizards
void draw_body(WINDOW *win, int pos_x, int pos_y, direction_t direction);

// Function to clear display
void disp_clear_window(WINDOW *win);

#endif //display_funcs_H