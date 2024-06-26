#include <ncurses.h>
#include "display-funcs.h"
#include "../common-files/lizardsNroachesNwasps.h"


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

    // If points less than 0, don't draw body
    if (lizard.points < 0)
    {
        return;
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

void disp_update(thread_args *game)
{
    int i = 0;

    // Clear window
    disp_clear_window(game->game_win);
    wclear(game->lines_win);

   // Draw all npcs
    for (i = 0; i < game->n_npc; i++)
    {
        disp_draw_entity(game->game_win, game->npc_array[i]);
    }

    // Draw all lizards
    for (i = 0; i < game->n_lizards; i++)
    {
        draw_body(game->game_win, game->lizard_array[i]); // draw all bodys
    }
    // Draw head of lizards later so it always stays on top
    for (i = 0; i < game->n_lizards; i++)
    {
        disp_draw_entity(game->game_win, game->lizard_array[i]);
    }
    
    // Print scores
    for (int i = 0; i < game->n_lizards; i++)
    {
        mvwprintw(game->lines_win, i, 1, "%c: %d", game->lizard_array[i].ch, game->lizard_array[i].points);
    }
    // Update display
    wrefresh(game->game_win);
    wrefresh(game->lines_win);
}
