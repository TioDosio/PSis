#include "lizardsNroaches.h"
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

    // send connection message
    generic_msg m;
    m.msg_type = 0; // connection msg
    m.entity_type = 0;
    m.ch = ' ';
    m.secret_code = -1;
    m.direction = UP;
    rc = zmq_send(requester, &m, sizeof(m), 0);
    assert(rc != -1);

    rc = zmq_recv(requester, &r, sizeof(r), 0);
    assert(rc != -1);

    if (r.success == 0)
    {
        printf("Server Full, try again later\n");
        exit(0);
    }
    initscr(); /* Start curses mode 		*/
    cbreak();  /* Line buffering disabled	*/
    mvprintw(0, 0, "Received reply: %d", r.success);
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */

    int n = 0;

    int key;
    do
    {
        //  prepare the movement message
        m.msg_type = 1;
        m.ch = ' ';
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
            m.direction = LEFT;
            break;
        case KEY_RIGHT:
            mvprintw(0, 0, "%d Right arrow is pressed", n);
            //  prepare the movement message
            m.direction = RIGHT;
            break;
        case KEY_DOWN:
            mvprintw(0, 0, "%d Down arrow is pressed", n);
            //  prepare the movement message
            m.direction = DOWN;
            break;
        case KEY_UP:
            mvprintw(0, 0, "%d :Up arrow is pressed", n);
            //  prepare the movement message
            m.direction = UP;
            break;
        case 'q':
        case 'Q':
            m.msg_type = 2;
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