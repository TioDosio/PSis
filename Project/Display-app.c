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

int main(int argc, char *argv[])
{
    char server_address[256];
    char *server_ip = "127.0.0.1";
    char *server_port = "6669";
    // para colocar o ip e a porta como argumentos
    if (argc != 3)
    {
        printf("You insert %d arguments, you need 3\n", argc);
    }
    else
    {
        server_ip = argv[1];
        server_port = argv[2];
    }

    display_update update;
    update.n_lizards = 0;
    update.n_roaches = 0;

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    snprintf(server_address, sizeof(server_address), "tcp://%s:%s", server_ip, server_port);
    zmq_connect(subscriber, server_address);
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

    int n_roaches = 0;
    int n_lizards = 0;

    while (1)
    {
        char *cap[256];
        zmq_recv(subscriber, cap, 3, 0);
        int i;

        //Clear the screen
        wclear(my_win);
        box(my_win, 0, 0);
        wrefresh(my_win);

        zmq_recv(subscriber, &update, sizeof(update), 0);

        n_roaches = update.n_roaches;
        n_lizards = update.n_lizards;

        for (i = 0; i < n_roaches; i++)
        {  
            disp_draw_entity(my_win, update.roach[i]);
        }
        for (i = 0; i < n_lizards; i++)
        {
            draw_body(my_win, update.lizard[i].pos_x, update.lizard[i].pos_y, update.lizard[i].direction);
        }
        for (i = 0; i < n_lizards; i++)
        {
            disp_draw_entity(my_win, update.lizard[i]);
        }
        wrefresh(my_win);
    }
    endwin(); /* End curses mode */
}
