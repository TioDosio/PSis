#include "lizardsNroaches.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <zmq.h>

int main(int argc, char *argv[])
{
    // para colocar o ip e a porta como argumentos
    /*if (argc != 3)
    {
        printf("You insert %s arguments, you need 3\n", argc);
        return 1; // Return an error code
    }*/

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, ADDRESS_RC);
    response_msg r;
    //  read the character from the user
    char ch;
    do
    {
        printf("what is your character(a..z)?: ");
        ch = getchar();
        ch = tolower(ch);
    } while (!isalpha(ch));

    // send connection message
    generic_msg m;
    m.msg_type = 0; // connection msg
    m.entity_type = 0;
    m.ch = ch;
    zmq_send(requester, &m, sizeof(m), 0);

    initscr();            /* Start curses mode 		*/
    cbreak();             /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */

    int n = 0;

    int key;
    do
    {
        //  prepare the movement message
        m.msg_type = 1;
        m.ch = ch;
        m.direction = 0;

        key = getch();
        usleep(10000);
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
            zmq_send(requester, &m, sizeof(m), 0); // adios
            exit(0);
            break;
        case 'Q':
            zmq_send(requester, &m, sizeof(m), 0); // adios
            exit(0);
            break;
        default:
            key = 'x';
            break;
        }

        // send the movement message
        if (key != 'x')
        {
            zmq_send(requester, &m, sizeof(m), 0);
            zmq_recv(requester, &r, sizeof(r), 0);
            mvprintw(1, 0, "Received %d", r.success);
        }
        refresh(); /* Print it on to the real screen */
    } while (key != 27);

    zmq_close(requester);
    zmq_ctx_destroy(context);
    endwin(); /* End curses mode		  */

    return 0;
}