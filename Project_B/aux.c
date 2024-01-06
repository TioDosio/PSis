#include "../common-files/lizardsNroachesNwasps.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <zmq.h>
#include <time.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    char server_address[256];
    char *server_ip;
    char *server_port;
    // para colocar o ip e a porta como argumentos
    if (argc == 3)
    {
        server_ip = argv[1];
        server_port = argv[2];
    }
    else
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]); // "Usage: %s <server_ip> <server_port>\n
        return 1;
    }

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ); // "ipc://127.0.0.1:6666"
    snprintf(server_address, sizeof(server_address), "tcp://%s:%s", server_ip, server_port);
    int rc;
    rc = zmq_connect(requester, server_address);
    assert(rc != -1);
    response_msg r;
    connect_display_resp disp_start_msg;

    // send connection message
    client_msg m;
    m.msg_type = CONNECT;
    m.entity_type = LIZARD;
    m.content = 0;
    m.secret_code = -1;
    rc = zmq_send(requester, &m, sizeof(m), 0);
    assert(rc != -1);

    rc = zmq_recv(requester, &disp_start_msg, sizeof(disp_start_msg), 0);
    assert(rc != -1);

    if (disp_start_msg.success == 0)
    {
        printf("Server Full, try again later\n");
        exit(0);
    }

    // Load game variables
    int secret_code = disp_start_msg.secret_code;
    
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

    entity_t *array_lizards = disp_start_msg.lizard;
    entity_t *array_npc = disp_start_msg.npc;

    int n_npc = disp_start_msg.n_npc;
    int n_lizards = disp_start_msg.n_lizards;
    int i;

    // first draw
    for (i = 0; i < n_npc; i++)
    {
        disp_draw_entity(my_win, array_npc[i]);
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

    int n = 0;

    int key;
    do
    {
        //  prepare the movement message
        m.msg_type = 1;
        m.content = 0;
        m.secret_code = r.secret_code;
        key = getch();
        usleep(10000);
        clear();
        n++;
        switch (key)
        {
        case KEY_LEFT:
            mvprintw(0, 0, "%d Left arrow is pressed", n);
            //  prepare the movement message
            m.content = LEFT;
            break;
        case KEY_RIGHT:
            mvprintw(0, 0, "%d Right arrow is pressed", n);
            //  prepare the movement message
            m.content = RIGHT;
            break;
        case KEY_DOWN:
            mvprintw(0, 0, "%d Down arrow is pressed", n);
            //  prepare the movement message
            m.content = DOWN;
            break;
        case KEY_UP:
            mvprintw(0, 0, "%d :Up arrow is pressed", n);
            //  prepare the movement message
            m.content = UP;
            break;
        case 'q':
        case 'Q':
            m.msg_type = DISCONNECT;
            break;
        default:
            key = 'x';
            break;
        }

        // send the movement message
        if (key != 'x')
        {
            if (zmq_send(requester, &m, sizeof(m), 0) == -1)
            {
                continue;
            }
            if (zmq_recv(requester, &r, sizeof(r), 0) == -1)
            {
                continue;
            }

            mvprintw(2, 0, "Score: %d", r.score);
            if (r.success == 2)
            {
                mvprintw(2, 0, "Disconnected from server\n");
                refresh();
                key = 27;
                break;
            }
        }
        refresh(); /* Print it on to the real screen */
    } while (key != 27);

    zmq_close(requester);
    zmq_ctx_destroy(context);
    endwin(); /* End curses mode		  */

    return 0;
}