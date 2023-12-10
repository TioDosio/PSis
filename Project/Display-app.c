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

int main()
{
    char server_address[] = "tcp://127.0.0.1:6666";

    connect_display_resp resp_connect;
    generic_msg msg_connect;
    display_update update;

    resp_connect.n_lizards = 0;
    resp_connect.n_roaches = 0;
    int rc;

    // creating request socket REQ-REP
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    rc = zmq_connect(requester, server_address);
    assert(rc != -1);

    // send connection message
    msg_connect.entity_type = DISPLAY;
    /*Irrelevant values*/
    msg_connect.msg_type = 0;
    msg_connect.ch = ' ';
    msg_connect.direction = UP;
    msg_connect.secret_code = 0;
    /*Irrelevant values*/

    rc = zmq_send(requester, &msg_connect, sizeof(msg_connect), 0);
    assert(rc != -1);
    rc = zmq_recv(requester, &resp_connect, sizeof(resp_connect), 0);
    assert(rc != -1);

    // creating request socket PUB-SUB
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    rc = zmq_connect(subscriber, resp_connect.address_port);
    assert(rc != -1);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "dis", 3);

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

    entity_t *array_lizards = resp_connect.lizard;
    entity_t *array_roaches = resp_connect.roach;

    int n_roaches = resp_connect.n_roaches;
    int n_lizards = resp_connect.n_lizards;
    int i;
    // first draw
    for (i = 0; i < n_roaches; i++)
    {
        disp_draw_entity(my_win, array_roaches[i]);
    }
    for (i = 0; i < n_lizards; i++)
    {
        draw_body(my_win, array_lizards[i]);
    }
    for (i = 0; i < n_lizards; i++)
    {
        disp_draw_entity(my_win, array_lizards[i]);
    }
    wrefresh(my_win);

    while (1)
    {
        char *cap[4];
        rc = zmq_recv(subscriber, cap, 3, 0);
        assert(rc != -1);
        int i;

        // Clear the screen
        wclear(my_win);
        box(my_win, 0, 0);
        wrefresh(my_win);

        rc = zmq_recv(subscriber, &update, sizeof(update), 0);
        assert(rc != -1);
        short id = -1;
        switch (update.entity.entity_type)
        {
        case 0:
            id = find_entity_id(array_lizards, n_lizards, update.entity.secret_code);
            if (id == -1) // If it's a new lizard, add it to the array
            {
                array_lizards[n_lizards] = update.entity;
                n_lizards++;
            }
            else
            {
                array_lizards[id] = update.entity;
                if (update.disconnect == 1)
                {
                    remove_entity(array_lizards, &n_lizards, id);
                }
            }
            break;
        case 1:
            id = find_entity_id(array_roaches, n_roaches, update.entity.secret_code);
            if (id == -1) // If it's a new roach, add it to the array
            {
                array_roaches[n_roaches] = update.entity;
                n_roaches++;
            }
            else
            {
                array_roaches[id] = update.entity;
                if (update.disconnect == 1)
                {
                    remove_entity(array_roaches, &n_roaches, id);
                }
            }
            break;

        default:
            id = -1;
            continue;
            break;
        }

        // Check if roaches got eaten
        for (int i = 0; i < n_roaches; i++)
        {
            for (int j = 0; j < n_lizards; j++)
            {
                if (array_lizards[j].pos_x == array_roaches[i].pos_x && array_lizards[j].pos_y == array_roaches[i].pos_y)
                {
                    array_roaches[i].ch = ' ';
                }
            }
        }
        // Check if lizards got eaten
        if (update.id_l_bumped != -1)
        {
            array_lizards[update.id_l_bumped].points = update.entity.points;
        }
        // desenhar tudo porque bodys e sobreposições
        for (i = 0; i < n_roaches; i++)
        {
            disp_draw_entity(my_win, array_roaches[i]);
        }
        for (i = 0; i < n_lizards; i++)
        {
            draw_body(my_win, array_lizards[i]);
        }
        for (i = 0; i < n_lizards; i++)
        {
            disp_draw_entity(my_win, array_lizards[i]);
        }

        wclear(lines_win);
        // Display Scores
        for (int i = 0; i < n_lizards; i++)
        {
            mvwprintw(lines_win, i, 1, "%c: %d", array_lizards[i].ch, array_lizards[i].points);
        }
        wmove(lines_win, 0, 0);
        wrefresh(my_win);
        wrefresh(lines_win);
    }
    endwin(); /* End curses mode */
}
