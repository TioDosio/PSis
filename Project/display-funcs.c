#include "lizardsNroaches.h"
#include <ncurses.h>
#include "display-funcs.h"

// Function to clear entity from display
void disp_clear_entity(WINDOW *win, entity_t entity)
{
    // get values
    int x = entity.pos_x;
    int y = entity.pos_y;

    /*deletes old place */
    wmove(win, x, y);
    waddch(win, ' ');

    if (entity.entity_type == LIZARD)
    {
        clear_body(win, x, y, entity.direction);
    }
}

// Function to draw entity on display
void disp_draw_entity(WINDOW *win, entity_t entity)
{
    // get values
    int x = entity.pos_x;
    int y = entity.pos_y;
    char ch = entity.ch;

    /*write head */
    wmove(win, x, y);
    waddch(win, ch | A_BOLD);
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
            if (pos_x + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x + 3, pos_y, " ");
                if (pos_x + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x + 4, pos_y, " ");
                    if (pos_x + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x + 5, pos_y, " ");
                    }
                }
            }
        }

        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, " ");
        if (pos_x - 2 > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, " ");
            if (pos_x - 3 > 0)
            {
                mvwprintw(win, pos_x - 3, pos_y, " ");
                if (pos_x - 4 > 0)
                {
                    mvwprintw(win, pos_x - 4, pos_y, " ");
                    if (pos_x - 5 > 0)
                    {
                        mvwprintw(win, pos_x - 5, pos_y, " ");
                    }
                }
            }
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, " ");
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, " ");
            if (pos_y + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x, pos_y + 3, " ");
                if (pos_y + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x, pos_y + 4, " ");
                    if (pos_y + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x, pos_y + 5, " ");
                    }
                }
            }
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, " ");
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, " ");
            if (pos_y - 3 > 0)
            {
                mvwprintw(win, pos_x, pos_y - 3, " ");
                if (pos_y - 4 > 0)
                {
                    mvwprintw(win, pos_x, pos_y - 4, " ");
                    if (pos_y - 5 > 0)
                    {
                        mvwprintw(win, pos_x, pos_y - 5, " ");
                    }
                }
            }
        }

        break;
    default:
        break;
    }
}

void draw_body(WINDOW *win, entity_t lizard)
{
    int pos_x = lizard.pos_x;
    int pos_y = lizard.pos_y;
    direction_t direction = lizard.direction;
    char c[2];
    c[0] = '.';
    c[1] = '\0';

    // If points more than 50, use c = '*'
    if (lizard.points >= 50)
    {
        c[0] = '*';
    }

    
    switch (direction)
    {
    case UP:
        if (pos_x + 1 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 1, pos_y, c);
            if (pos_x + 2 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x + 2, pos_y, c);
                if (pos_x + 3 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x + 3, pos_y, c);
                    if (pos_x + 4 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x + 4, pos_y, c);
                        if (pos_x + 5 != WINDOW_SIZE - 1)
                        {
                            mvwprintw(win, pos_x + 5, pos_y, c);
                        }
                    }
                }
            }
        }

        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, c);
        if ((pos_x - 2) > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, c);
            if ((pos_x - 3) > 0)
            {
                mvwprintw(win, pos_x - 3, pos_y, c);
                if ((pos_x - 4) > 0)
                {
                    mvwprintw(win, pos_x - 4, pos_y, c);
                    if ((pos_x - 5) > 0)
                    {
                        mvwprintw(win, pos_x - 5, pos_y, c);
                    }
                }
            }
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, c);
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, c);
            if (pos_y + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x, pos_y + 3, c);
                if (pos_y + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x, pos_y + 4, c);
                    if (pos_y + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x, pos_y + 5, c);
                    }
                }
            }
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, c);
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, c);
            if (pos_y - 3 > 0)
            {
                mvwprintw(win, pos_x, pos_y - 3, c);
                if (pos_y - 4 > 0)
                {
                    mvwprintw(win, pos_x, pos_y - 4, c);
                    if (pos_y - 5 > 0)
                    {
                        mvwprintw(win, pos_x, pos_y - 5, c);
                    }
                }
            }
        }

        break;
    default:
        break;
    }
}

void disp_clear_window(WINDOW *win)
{
    wclear(win);
    box(win, 0, 0);
}

