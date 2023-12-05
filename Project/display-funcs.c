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

void draw_body(WINDOW *win, int pos_x, int pos_y, direction_t direction)
{
    switch (direction)
    {
    case UP:
        mvwprintw(win, pos_x + 1, pos_y, ".");
        if (pos_x + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 2, pos_y, ".");
            if (pos_x + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x + 3, pos_y, ".");
                if (pos_x + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x + 4, pos_y, ".");
                    if (pos_x + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x + 5, pos_y, ".");
                    }
                }
            }
        }

        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, ".");
        if ((pos_x - 2) > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, ".");
            if ((pos_x - 3) > 0)
            {
                mvwprintw(win, pos_x - 3, pos_y, ".");
                if ((pos_x - 4) > 0)
                {
                    mvwprintw(win, pos_x - 4, pos_y, ".");
                    if ((pos_x - 5) > 0)
                    {
                        mvwprintw(win, pos_x - 5, pos_y, ".");
                    }
                }
            }
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, ".");
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, ".");
            if (pos_y + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x, pos_y + 3, ".");
                if (pos_y + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x, pos_y + 4, ".");
                    if (pos_y + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x, pos_y + 5, ".");
                    }
                }
            }
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, ".");
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, ".");
            if (pos_y - 3 > 0)
            {
                mvwprintw(win, pos_x, pos_y - 3, ".");
                if (pos_y - 4 > 0)
                {
                    mvwprintw(win, pos_x, pos_y - 4, ".");
                    if (pos_y - 5 > 0)
                    {
                        mvwprintw(win, pos_x, pos_y - 5, ".");
                    }
                }
            }
        }

        break;
    default:
        break;
    }
}