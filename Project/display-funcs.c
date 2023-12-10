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
    char c = '.';

    // If points more than 50, use c = '*'
    if (lizard.points >= 50)
    {
        c = '*';
    }

    switch (direction)
    {
    case UP:
        if (pos_x + 1 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x + 1, pos_y, "%c", c);
            if (pos_x + 2 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x + 2, pos_y, "%c", c);
                if (pos_x + 3 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x + 3, pos_y, "%c", c);
                    if (pos_x + 4 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x + 4, pos_y, "%c", c);
                        if (pos_x + 5 != WINDOW_SIZE - 1)
                        {
                            mvwprintw(win, pos_x + 5, pos_y, "%c", c);
                        }
                    }
                }
            }
        }

        break;
    case DOWN:
        mvwprintw(win, pos_x - 1, pos_y, "%c", c);
        if ((pos_x - 2) > 0)
        {
            mvwprintw(win, pos_x - 2, pos_y, "%c", c);
            if ((pos_x - 3) > 0)
            {
                mvwprintw(win, pos_x - 3, pos_y, "%c", c);
                if ((pos_x - 4) > 0)
                {
                    mvwprintw(win, pos_x - 4, pos_y, "%c", c);
                    if ((pos_x - 5) > 0)
                    {
                        mvwprintw(win, pos_x - 5, pos_y, "%c", c);
                    }
                }
            }
        }

        break;
    case LEFT:
        mvwprintw(win, pos_x, pos_y + 1, "%c", c);
        if (pos_y + 2 != WINDOW_SIZE - 1)
        {
            mvwprintw(win, pos_x, pos_y + 2, "%c", c);
            if (pos_y + 3 != WINDOW_SIZE - 1)
            {
                mvwprintw(win, pos_x, pos_y + 3, "%c", c);
                if (pos_y + 4 != WINDOW_SIZE - 1)
                {
                    mvwprintw(win, pos_x, pos_y + 4, "%c", c);
                    if (pos_y + 5 != WINDOW_SIZE - 1)
                    {
                        mvwprintw(win, pos_x, pos_y + 5, "%c", c);
                    }
                }
            }
        }

        break;
    case RIGHT:
        mvwprintw(win, pos_x, pos_y - 1, "%c", c);
        if (pos_y - 2 > 0)
        {
            mvwprintw(win, pos_x, pos_y - 2, "%c", c);
            if (pos_y - 3 > 0)
            {
                mvwprintw(win, pos_x, pos_y - 3, "%c", c);
                if (pos_y - 4 > 0)
                {
                    mvwprintw(win, pos_x, pos_y - 4, "%c", c);
                    if (pos_y - 5 > 0)
                    {
                        mvwprintw(win, pos_x, pos_y - 5, "%c", c);
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

// returns correct position of the entity from the array
int find_entity_id(entity_t entity[], int n_entities, int code)
{
    for (int i = 0; i < n_entities; i++)
    {
        if (code == entity[i].secret_code)
        {
            return i;
        }
    }
    return -1;
}

void remove_entity(entity_t entity[], int *n_entities, int id)
{
    for (int i = id; i < *n_entities - 1; i++)
    {
        entity[i] = entity[i + 1];
    }
    *n_entities = *n_entities - 1;
}