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
#include <string.h>

void generate_r(response_msg *r, int suc, int code, int score)
{
    r->success = suc;
    r->secrect_code = code;
    r->score = score;
}

int generate_code()
{
    return rand() % 10000;
}

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
int find_entity_id(entity_t entity[], int n_entities, int code)
{
    for (int i = 0; i < n_entities; i++)
    {
        if (code == entity[i].secrect_code)
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
    for (int i = 0; i < MAX_LIZARDS; i++)
    {
        lizard_array[i].secrect_code = -1;
    }
    for (int i = 0; i < MAX_ROACHES; i++)
    {
        roach_array[i].secrect_code = -1;
    }
    int n_roaches = 0;
    int n_lizards = 0;
    srand(time(NULL));

    generic_msg m;
    response_msg r;
    display_update update;

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP); // Create socket por REQ-REP
    void *publisher = zmq_socket(context, ZMQ_PUB); // Create socket for PUB-SUB

    int rc = zmq_bind(responder, ADDRESS_REQ); // Bind to address
    assert(rc == 0);

    rc = zmq_bind(publisher, ADDRESS_PUB); // Bind to address
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

    int code;

    while (1)
    {
        zmq_recv(responder, &m, sizeof(m), 0);
        if (m.msg_type == 0) // if connection request
        {
            // Generate Secrete code
            code = generate_code();
            switch (m.entity_type)
            {
            case LIZARD:
                if (n_lizards < MAX_LIZARDS)
                {
                    // Save values to Array
                    lizard_array[n_lizards].entity_type = m.entity_type;
                    lizard_array[n_lizards].ch = m.ch;
                    lizard_array[n_lizards].points = 0;
                    lizard_array[n_lizards].pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
                    lizard_array[n_lizards].pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
                    lizard_array[n_lizards].secrect_code = code;
                    lizard_array[n_lizards].direction = m.direction;
                    n_lizards++;
                    generate_r(&r, 1, code, 0);
                }
                else
                {
                    r.success = 0;
                }
                break;

            case ROACH:
                if (n_roaches < MAX_ROACHES)
                {
                    // Save values to Array
                    roach_array[n_roaches].entity_type = m.entity_type;
                    roach_array[n_roaches].ch = m.ch;
                    roach_array[n_roaches].points = m.ch - '0';
                    roach_array[n_roaches].pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
                    roach_array[n_roaches].pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
                    roach_array[n_roaches].secrect_code = code;
                    roach_array[n_roaches].direction = m.direction;

                    n_roaches++;
                    generate_r(&r, 1, code, 0);
                }
                else
                {
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
            entity_t old_entity; //aux variable to store current values
            entity_t new_entity; //aux variable to store new values
            code = m.secrect_code;
            switch (m.entity_type)
            {
            case LIZARD:
                entity_id = find_entity_id(lizard_array, n_lizards, code);
                if (entity_id != -1)
                {
                    old_entity = lizard_array[entity_id]; // save old values
                }

                break;
            case ROACH:
                entity_id = find_entity_id(roach_array, n_roaches, code);
                old_entity = roach_array[entity_id];
                break;
            default:
                generate_r(&r, 0, code, 0);
                entity_id = -1;
                break;
            }

            // Common procedure for both Roaches and Lizards
            if (entity_id != -1)
            {
                r.secrect_code = old_entity.secrect_code;
                pos_x = old_entity.pos_x;
                pos_y = old_entity.pos_y;

                // Copy old values to new entity
                new_entity = old_entity;

                // update new direction rom message
                new_entity.direction = m.direction;
                new_position(&pos_x, &pos_y, m.direction);
                new_entity.pos_x = pos_x;
                new_entity.pos_y = pos_y;

                // Save values in array
                switch (m.entity_type)
                {
                case LIZARD:
                    lizard_array[entity_id] = new_entity;
                    generate_r(&r, 1, code, new_entity.points);
                    break;
                case ROACH:
                    roach_array[entity_id] = new_entity;
                    generate_r(&r, 1, code, 0);
                    break;
                }
            }
        }
        else if (m.msg_type == 2)
        {
            code = m.secrect_code;
            int entity_id = find_entity_id(lizard_array, n_lizards, code);
            if (entity_id != -1)
            {
                for (int i = entity_id; i < n_lizards - 1; i++)
                {
                    lizard_array[i] = lizard_array[i + 1];
                }
                n_lizards--;
                r.success = 2;
            }
        }
        //Draw entities on empty screen and create display update message
        disp_clear_window(my_win);
        int i = 0;
        update.n_lizards = n_lizards;
        update.n_roaches = n_roaches;

        for (i = 0; i < n_roaches; i++)
        {
            disp_draw_entity(my_win, roach_array[i]);
            update.roach[i] = roach_array[i];
        }

        for (i = 0; i < n_lizards; i++)
        {
            draw_body(my_win, lizard_array[i].pos_x, lizard_array[i].pos_y, lizard_array[i].direction);
            update.lizard[i] = lizard_array[i];
        }
        // Draw head of lizards later so it always stays on top
        for (i = 0; i < n_lizards; i++)
        {
            disp_draw_entity(my_win, lizard_array[i]);
        }

        // Update display
        wrefresh(my_win);
        //Send reply
        zmq_send(responder, &r, sizeof(r), 0);

        //Send display update
        char *string = "dis";
        zmq_send(publisher, string, strlen(string), ZMQ_SNDMORE);
        zmq_send(publisher, &update, sizeof(update), 0);
    }
    endwin(); /* End curses mode */

    return 0;
}
