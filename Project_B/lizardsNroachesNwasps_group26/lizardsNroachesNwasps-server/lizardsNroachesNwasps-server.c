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
#include "server-func.h"

time_t current_time;

int main()
{
    // Define lizards and roaches
    entity_t lizard_array[MAX_LIZARDS];
    entity_t roach_array[MAX_NPCS];
    time_t roach_death_time[MAX_NPCS];

    for (int i = 0; i < MAX_LIZARDS; i++)
    {
        lizard_array[i].secret_code = -1;
    }
    for (int i = 0; i < MAX_NPCS; i++)
    {
        roach_array[i].secret_code = -1;
    }
    int n_roaches = 0;
    int n_lizards = 0;

    srand(time(NULL));

    generic_msg m;
    response_msg r;
    display_update update;
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

    // Create another window for lines below the box
    WINDOW *lines_win = newwin(26, WINDOW_SIZE, WINDOW_SIZE, 0);

    int code;
    int skip_reply = 0; // Used to skip generic reply when sending display reply

    while (1)
    {
        skip_reply = 0;
        if (zmq_recv(responder, &m, sizeof(m), 0) == -1)
        {
            continue;
        }

        current_time = time(NULL); // get current time

        if (m.msg_type == 0) // if connection request
        {
            // Generate Secrete code
            code = generate_code(lizard_array, roach_array, n_lizards, n_roaches);
            switch (m.entity_type)
            {
            case LIZARD:
                if (n_lizards < MAX_LIZARDS)
                {
                    // Save values to Array
                    lizard_array[n_lizards].entity_type = m.entity_type;
                    lizard_array[n_lizards].ch = generateRandomChar();
                    lizard_array[n_lizards].points = 0;
                    lizard_array[n_lizards].pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
                    lizard_array[n_lizards].pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
                    lizard_array[n_lizards].secret_code = code;
                    lizard_array[n_lizards].direction = m.direction;

                    // Send new entity to display
                    update.entity = lizard_array[n_lizards];
                    update.disconnect = 0;

                    n_lizards++;
                    generate_r(&r, 1, code, 0);
                }
                else
                {
                    r.success = 0;
                }
                break;

            case ROACH:
                if (n_roaches < MAX_NPCS)
                {
                    // Save values to Array
                    roach_array[n_roaches].entity_type = m.entity_type;
                    roach_array[n_roaches].points = m.ch - '0';
                    roach_array[n_roaches].ch = m.ch;
                    roach_array[n_roaches].pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
                    roach_array[n_roaches].pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
                    roach_array[n_roaches].secret_code = code;
                    roach_array[n_roaches].direction = m.direction;

                    // Send new entity to display
                    update.entity = roach_array[n_roaches];
                    update.disconnect = 0;

                    n_roaches++;
                    generate_r(&r, 1, code, 0);
                }
                else
                {
                    r.success = 0;
                }
                break;
            case DISPLAY:
                // Write response message
                resp_connect.n_lizards = n_lizards;
                resp_connect.n_roaches = n_roaches;
                strncpy(resp_connect.address_port, ADDRESS_PUB, BUFFER_SIZE);
                for (int i = 0; i < n_lizards; i++)
                {
                    resp_connect.lizard[i] = lizard_array[i];
                }
                for (int i = 0; i < n_roaches; i++)
                {
                    resp_connect.roach[i] = roach_array[i];
                }
                // Send response message
                if (zmq_send(responder, &resp_connect, sizeof(resp_connect), 0) == -1)
                {
                    continue;
                }
                skip_reply = 1;
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
            int id = -1, id_bumped = -1;
            entity_t *moved_entity = move_entity(m, &r, array_entity, lizard_array, n_entity, n_lizards, &id, &id_bumped);
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
            generate_r(&r, 1, moved_entity->secret_code, moved_entity->points);
            update.entity = *moved_entity;
            update.disconnect = 0;
            update.id_l_bumped = id_bumped;
        }
        else if (m.msg_type == 2) // if disconnect request from lizards
        {
            code = m.secret_code;
            int entity_id = find_entity_id(lizard_array, n_lizards, code);
            if (entity_id != -1)
            {
                generate_r(&r, 2, code, 0);
                update.entity = lizard_array[entity_id];
                update.disconnect = 1;
                update.id_l_bumped = -1;

                for (int i = entity_id; i < n_lizards - 1; i++)
                {
                    lizard_array[i] = lizard_array[i + 1];
                }
                n_lizards--;
            }
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

        // Skip response if it is a display reply
        if (skip_reply == 1)
        {
            skip_reply = 0;
            continue;
        }
        // Send response message
        if (zmq_send(responder, &r, sizeof(r), 0) == -1)
        {
            code = m.secret_code;
            if (m.entity_type == LIZARD)
            {
                int entity_id = find_entity_id(lizard_array, n_lizards, code);
                if (entity_id != -1)
                {
                    remove_entity(lizard_array, &n_lizards, entity_id);
                    update.entity = lizard_array[entity_id];
                    update.id_l_bumped = -1;
                }
            }
            else if (m.entity_type == ROACH)
            {
                int entity_id = find_entity_id(roach_array, n_roaches, code);
                for (int i = 0; i < n_roaches; i++)
                {
                    if (roach_array[i].secret_code == code)
                    {
                        remove_entity(roach_array, &n_roaches, i);
                        update.entity = roach_array[entity_id];
                        update.id_l_bumped = -1;
                    }
                }
            }
            update.disconnect = 1;
        }
        wclear(lines_win);
        for (int i = 0; i < n_lizards; i++)
        {
            mvwprintw(lines_win, i, 1, "%c: %d", lizard_array[i].ch, lizard_array[i].points);
        }
        wmove(lines_win, 0, 0);
        wrefresh(lines_win);

        // Send display update
        char *string = "dis";
        if (zmq_send(publisher, string, strlen(string), ZMQ_SNDMORE) == -1)
        {
            continue;
        }
        if (zmq_send(publisher, &update, sizeof(update), 0) == -1)
        {
            continue;
        }
    }
    endwin(); /* End curses mode */

    return 0;
}
