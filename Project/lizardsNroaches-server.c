#include <zmq.h>
#include <ncurses.h>
#include "lizardsNroaches.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "display-funcs.h"

void new_position(int *x, int *y, direction_t direction)
{
    switch (direction)
    {
    case UP:
        (*x)--;
        if (*x == 0)
            *x = 2;
        break;
    case DOWN:
        (*x)++;
        if (*x == WINDOW_SIZE - 1)
            *x = WINDOW_SIZE - 3;
        break;
    case LEFT:
        (*y)--;
        if (*y == 0)
            *y = 2;
        break;
    case RIGHT:
        (*y)++;
        if (*y == WINDOW_SIZE - 1)
            *y = WINDOW_SIZE - 3;
        break;
    default:
        break;
    }
}

// returns correct position of the entity from the array
int find_entity_id(entity_t entity[], int n_entities, int ch)
{
    for (int i = 0; i < n_entities; i++)
    {
        if (ch == entity[i].ch)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // Define lizards and roaches
    entity_t lizard_array[MAX_LIZARDS];
    entity_t roach_array[MAX_ROACHES];
    int n_roaches = 0;
    int n_lizards = 0;
    srand(time(NULL));

    generic_msg m;
    response_msg r;

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, ADDRESS_RC);
    assert(rc == 0);

    // Initialize the screen
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    /* creates a window and draws a border */
    WINDOW *my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0, 0);
    wrefresh(my_win);

    int pos_x;
    int pos_y;

    while (1)
    {
        zmq_recv(responder, &m, sizeof(m), 0);
        if (m.msg_type == 0) // if connection request
        {
            r.success = 0;

            switch (m.entity_type)
            {
            case LIZARD:
                if (n_lizards <= MAX_LIZARDS)
                {
                    lizard_array[n_lizards].entity_type = m.entity_type;
                    lizard_array[n_lizards].ch = m.ch;
                    lizard_array[n_lizards].points = 0;
                    lizard_array[n_lizards].pos_x = rand() % (WINDOW_SIZE - 1) + 1;
                    lizard_array[n_lizards].pos_y = rand() % (WINDOW_SIZE - 1) + 1;

                    disp_draw_entity(my_win, lizard_array[n_lizards]);

                    n_lizards++;
                    r.success = 1;
                }
                else
                {
                    // mandar response a dizer que nao pode
                    printf("Too many lizards\n");
                    r.success = 0;
                }
                break;

            case ROACH:
                if (n_roaches <= MAX_ROACHES)
                {
                    roach_array[n_roaches].entity_type = m.entity_type;
                    roach_array[n_roaches].ch = m.ch;
                    roach_array[n_roaches].points = m.ch - '0';
                    roach_array[n_roaches].pos_x = rand() % WINDOW_SIZE + 1;
                    roach_array[n_roaches].pos_y = rand() % WINDOW_SIZE + 1;

                    disp_draw_entity(my_win, roach_array[n_roaches]);

                    n_roaches++;
                    r.success = 1;
                }
                else
                {
                    // mandar response a dizer que nao pode
                    printf("Too many roaches\n");
                    r.success = 0;
                }
                break;

            default:
                break;
            }
        }
        else if (m.msg_type == 1) // if movement request
        {
            int entity_id;
            entity_t old_entity;
            entity_t new_entity;
            switch (m.entity_type)
            {
            case LIZARD:
                entity_id = find_entity_id(lizard_array, n_lizards, m.ch);
                if (entity_id != -1)
                {
                    old_entity = lizard_array[entity_id]; // save old values
                }
                break;
            case ROACH:
                old_entity = roach_array[entity_id]; // save old values
                break;
            default:
                r.success = 0;
                entity_id = -1;
                break;
            }

            // Common procedure for both Roaches and Lizards
            if (entity_id != -1)
            {
                pos_x = old_entity.pos_x;
                pos_y = old_entity.pos_y;

                // Copy old values to new entity
                new_entity = old_entity;

                // update new direction rom message
                new_entity.direction = m.direction;
                new_position(&pos_x, &pos_y, m.direction);
                new_entity.pos_x = pos_x;
                new_entity.pos_y = pos_y;

                // Delete old entity and draw new one
                disp_clear_entity(my_win, old_entity);
                disp_draw_entity(my_win, new_entity);

                // Save values in array
                switch (m.entity_type)
                {
                case LIZARD:
                    lizard_array[entity_id] = new_entity;
                    break;
                case ROACH:
                    roach_array[entity_id] = new_entity;
                    break;
                default:
                    break;
                }
                int i = 0;
                for (; i < n_lizards; i++)
                {
                    disp_clear_entity(my_win, lizard_array[i]);
                    disp_draw_entity(my_win, lizard_array[i]);
                }
                i = 0;
                for (; i < n_roaches; i++)
                {
                    disp_clear_entity(my_win, roach_array[i]);
                    disp_draw_entity(my_win, roach_array[i]);
                }
            }
        }
        wrefresh(my_win);
        zmq_send(responder, &r, sizeof(r), 0);
    }
    endwin(); /* End curses mode */

    return 0;
}
