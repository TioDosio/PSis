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

time_t current_time;

void generate_r(response_msg *r, int suc, int code, int score)
{
    r->success = suc;
    r->secret_code = code;
    r->score = score;
}

int generate_code()
{
    return rand() % 10000;
}

// Returns array pos if it is on the given position
// Returns -1 if there is no entity on the given position
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

entity_t *move_entity(generic_msg m, response_msg *r, entity_t array_entity[], int n_entity, int *entity_id)
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

    // For lizards, check if there is another lizard on the new position and undo movement if so
    if (m.entity_type == LIZARD)
    {
        int on_pos_id = on_pos(array_entity, n_entity, pos_x, pos_y);
        if (on_pos_id != -1)
        {
            // If there is another snake on the new position, do not move
            pos_x = old_entity.pos_x;
            pos_y = old_entity.pos_y;
            // And average points of both Snakes
            new_entity.points = (old_entity.points + array_entity[on_pos_id].points) / 2;
            array_entity[on_pos_id].points = new_entity.points;
        }
    }

    // Finish movement and update data
    new_entity.pos_x = pos_x;
    new_entity.pos_y = pos_y;

    array_entity[*entity_id] = new_entity;
    generate_r(r, 1, code, new_entity.points);

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
            lizard->points += roach_array[i].points;
            // turn roach invisible and pointless for five seconds
            roach_array[i].points = 0;
            roach_array[i].ch = ' ';
            roach_death_time[i] = time(NULL);
        }
    }
}

void respawn_roach(entity_t *roach)
{
    roach->points = (rand() % 4) + 1; // 1 to 4
    roach->ch = roach->points + '0';
    roach->pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
    roach->pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
}

int main()
{
    // Define lizards and roaches
    entity_t lizard_array[MAX_LIZARDS];
    entity_t roach_array[MAX_ROACHES];
    time_t roach_death_time[MAX_ROACHES];

    for (int i = 0; i < MAX_LIZARDS; i++)
    {
        lizard_array[i].secret_code = -1;
    }
    for (int i = 0; i < MAX_ROACHES; i++)
    {
        roach_array[i].secret_code = -1;
    }
    int n_roaches = 0;
    int n_lizards = 0;

    srand(time(NULL));

    generic_msg m;
    response_msg r;
    display_update update;
    connect_display connect;
    connect_display_resp resp_connect;

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

    int code;

    while (1)
    {
        if (zmq_recv(responder, &m, sizeof(m), 0) == -1)
        {
            continue;
        }

        current_time = time(NULL); // get current time
        if (m.msg_type == 0)       // if connection request
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
                    lizard_array[n_lizards].secret_code = code;
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
                    roach_array[n_roaches].secret_code = code;
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
            entity_t *array_entity;
            int n_entity;
            switch (m.entity_type)
            {
            case LIZARD:
                n_entity = n_lizards;
                array_entity = lizard_array;
                break;
            case ROACH:
                n_entity = n_roaches;
                array_entity = roach_array;
                break;
            default:
                array_entity = NULL;
                break;
            }
            int id = -1;
            entity_t *moved_entity = move_entity(m, &r, array_entity, n_entity, &id);
            if (id == -1 || moved_entity == NULL)
            {
                continue; // if entity not found, do nothing
            }

            if (m.entity_type == LIZARD)
            {
                eat_roaches(moved_entity, roach_array, n_roaches, roach_death_time);
            }
            else if (m.entity_type == ROACH && moved_entity->ch == ' ')
            {
                if (current_time - roach_death_time[id] >= ROACH_RESPAWN_TIME)
                {
                    respawn_roach(moved_entity);
                }
            }
            update.entity = *moved_entity;
        }
        else if (m.msg_type == 2) // if disconnect request from lizards
        {
            code = m.secret_code;
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

        if (connect.entity_type == 1)
        {
            resp_connect.n_lizards = n_lizards;
            resp_connect.n_roaches = n_roaches;
            resp_connect.address_port = ADDRESS_PUB;
            for (int i = 0; i < n_lizards; i++)
            {
                resp_connect.lizard[i] = lizard_array[i];
            }
            for (int i = 0; i < n_roaches; i++)
            {
                resp_connect.roach[i] = roach_array[i];
            }
            if (zmq_send(responder, &resp_connect, sizeof(resp_connect), 0) == -1)
            {
                continue;
            }
            connect.entity_type = 0;
        }
        // Draw entities on empty screen and create display update message
        disp_clear_window(my_win);
        int i = 0;

        for (i = 0; i < n_roaches; i++)
        {
            disp_draw_entity(my_win, roach_array[i]);
        }
        for (i = 0; i < n_lizards; i++)
        {
            draw_body(my_win, lizard_array[i]);
        }
        // Draw head of lizards later so it always stays on top
        for (i = 0; i < n_lizards; i++)
        {
            disp_draw_entity(my_win, lizard_array[i]);
        }

        // Update display
        wrefresh(my_win);
        // Send reply
        if (zmq_send(responder, &r, sizeof(r), 0) == -1)
        {
            if (m.entity_type == LIZARD)
            {
                code = m.secret_code;
                int entity_id = find_entity_id(lizard_array, n_lizards, code);
                if (entity_id != -1)
                {
                    for (int i = entity_id; i < n_lizards - 1; i++)
                    {
                        lizard_array[i] = lizard_array[i + 1];
                    }
                    n_lizards--;
                }
            }
            else if (m.entity_type == ROACH)
            {
                for (int i = 0; i < n_roaches; i++)
                {
                    if (roach_array[i].secret_code == code)
                    {
                        for (int j = i; j < n_roaches - 1; j++)
                        {
                            roach_array[j] = roach_array[j + 1];
                        }
                        n_roaches--;
                    }
                }
            }
        }

        // Send display update
        char *string = "dis";
        if (zmq_send(publisher, string, strlen(string), ZMQ_SNDMORE) == -1)
        {
            continue;
        }
        zmq_send(publisher, &update, sizeof(update), 0);
        {
            continue;
        }
    }
    endwin(); /* End curses mode */

    return 0;
}
