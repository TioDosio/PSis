#include <zmq.h>
#include <ncurses.h>
#include "lizardsNroaches.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

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

// returns the position of the entity in the array
int find_entity_id(entity_t char_data[], int n_char, int ch)
{
    for (int i = 0; i < n_char; i++)
    {
        if (ch == char_data[i].ch)
        {
            return i;
        }
    }
    return -1;
}

void draw_body(WINDOW *win, int pos_x, int pos_y, direction_t direction)
{
    switch (direction)
    {
    case UP:
        mvwprintw(win, pos_x + 1, pos_y, "*");
        if (pos_x + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 2, pos_y, "*");
        }
        if (pos_x + 3 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 3, pos_y, "*");
        }
        if (pos_x + 4 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 4, pos_y, "*");
        }
        if (pos_x + 5 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 5, pos_y, "*");
        }

        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, "*");
        if ((pos_x - 2) > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, "*");
        }
        if ((pos_x - 3) > 0)
        {
            mvwprintw(win, pos_x - 3, pos_y, "*");
        }
        if ((pos_x - 4) > 0)
        {
            mvwprintw(win, pos_x - 4, pos_y, "*");
        }
        if ((pos_x - 5) > 0)
        {
            mvwprintw(win, pos_x - 5, pos_y, "*");
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, "*");
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, "*");
        }
        if (pos_y + 3 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 3, "*");
        }
        if (pos_y + 4 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 4, "*");
        }
        if (pos_y + 5 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 5, "*");
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, "*");
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, "*");
        }
        if (pos_y - 3 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 3, "*");
        }
        if (pos_y - 4 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 4, "*");
        }
        if (pos_y - 5 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 5, "*");
        }
        break;
    default:
        break;
    }
}

void clear_body(WINDOW *win, int pos_x, int pos_y, direction_t direction)
{
    switch (direction)
    {
    case UP:
        mvwprintw(win, pos_x + 1, pos_y, " ");
        if (pos_x + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 2, pos_y, " ");
        }
        if (pos_x + 3 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 3, pos_y, " ");
        }
        if (pos_x + 4 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 4, pos_y, " ");
        }
        if (pos_x + 5 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 5, pos_y, " ");
        }
        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, " ");
        if (pos_x - 2 > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, " ");
        }
        if (pos_x - 3 > 0)
        {
            mvwprintw(win, pos_x - 3, pos_y, " ");
        }
        if (pos_x - 4 > 0)
        {
            mvwprintw(win, pos_x - 4, pos_y, " ");
        }
        if (pos_x - 5 > 0)
        {
            mvwprintw(win, pos_x - 5, pos_y, " ");
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, " ");
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, " ");
        }
        if (pos_y + 3 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 3, " ");
        }
        if (pos_y + 4 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 4, " ");
        }
        if (pos_y + 5 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 5, " ");
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, " ");
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, " ");
        }
        if (pos_y - 3 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 3, " ");
        }
        if (pos_y - 4 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 4, " ");
        }
        if (pos_y - 5 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 5, " ");
        }

        break;
    default:
        break;
    }
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

    int ch;
    int pos_x;
    int pos_y;

    direction_t direction;
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
        zmq_send(responder, &r, sizeof(r), 0);

        if (m.msg_type == 1) // if movement request
        {
            int ch_pos;
            switch (m.entity_type)
            {
            case LIZARD:
                ch_pos = find_entity_id(lizard_array, n_lizards, m.ch);
                if (ch_pos != -1)
                {
                    pos_x = lizard_array[ch_pos].pos_x;
                    pos_y = lizard_array[ch_pos].pos_y;
                    ch = lizard_array[ch_pos].ch;
                    /*deletes old place */
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win, ' ');
                    clear_body(my_win, pos_x, pos_y, direction);

                    /* calculates new direction */
                    direction = m.direction;

                    /* claculates new mark position */
                    new_position(&pos_x, &pos_y, direction);
                    lizard_array[ch_pos].pos_x = pos_x;
                    lizard_array[ch_pos].pos_y = pos_y;
                    draw_body(my_win, pos_x, pos_y, direction);
                }
                break;
            case ROACH:
                ch_pos = find_entity_id(roach_array, n_roaches, m.ch);
                if (ch_pos != -1)
                {
                    pos_x = roach_array[ch_pos].pos_x;
                    pos_y = roach_array[ch_pos].pos_y;
                    ch = roach_array[ch_pos].ch;
                    /*deletes old place */
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win, ' ');

                    /* claculates new direction */
                    direction = m.direction;

                    /* claculates new mark position */
                    new_position(&pos_x, &pos_y, direction);
                    roach_array[ch_pos].pos_x = pos_x;
                    roach_array[ch_pos].pos_y = pos_y;
                }
                break;
            default:
                break;
            }
            wmove(my_win, pos_x, pos_y);
            waddch(my_win, ch | A_BOLD);
            wrefresh(my_win);
        }
        /* draw mark on new position */
    }
    endwin(); /* End curses mode */

    return 0;
}
