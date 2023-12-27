#include "../common-files/lizardsNroaches.h"
#include <ncurses.h>
#include <time.h>

void generate_r(response_msg *r, int suc, int code, int score);

char generateRandomChar();

int generate_code(entity_t lizard_array[], entity_t roach_array[], int n_lizards, int n_roaches);

int on_pos(entity_t entity[], int n_entities, int x, int y);

void new_position(int *x, int *y, direction_t direction);

entity_t *move_entity(generic_msg m, response_msg *r, entity_t array_entity[], entity_t lizard_array[], int n_entity, int n_lizards, int *entity_id, int *id_bumped);

void eat_roaches(entity_t *lizard, entity_t roach_array[], int n, time_t roach_death_time[]);

void respawn_roach(entity_t *roach);
