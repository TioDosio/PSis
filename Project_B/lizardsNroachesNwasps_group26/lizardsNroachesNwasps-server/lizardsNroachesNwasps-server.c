#include <zmq.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include "../common-files/display-funcs.h"
#include <string.h>
#include <pthread.h>
#include "thread-funcs.h"

int main()
{
    // Define lizards, roaches and wasps
    entity_t lizard_array[MAX_LIZARDS];
    entity_t npc_array[MAX_NPCS];

    time_t roach_death_time[MAX_NPCS];

    for (int i = 0; i < MAX_LIZARDS; i++)
    {
        lizard_array[i].secret_code = -1;
    }

    for (int i = 0; i < MAX_NPCS; i++)
    {
        npc_array[i].secret_code = -1;
    }

    int n_lizards = 0;
    int n_npc = 0;

    //* Intitizalize ZMQ*//
    context = zmq_ctx_new ();

    //  Socket facing Lizard clients
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    int rc = zmq_bind (frontend, ADDRESS_REQ_LIZ);
    assert (rc == 0);
    //  Socket facing lizard threads
    void *backend = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend, BACK_END_ADDRESS);
    assert (rc == 0);

    // Initialize the screen
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    
    WINDOW *game_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    WINDOW *lines_win = newwin(MAX_LIZARDS, WINDOW_SIZE, WINDOW_SIZE, 0);

    /* creates a window and draws a border */
    box(game_win, 0, 0);
    wrefresh(game_win);

 
    // Create shared thread arguments
    thread_args shared;
    shared.lizard_array = lizard_array;
    shared.npc_array = npc_array;
    shared.n_lizards = n_lizards;
    shared.n_npc = n_npc;
    shared.roach_death_time = roach_death_time;
    shared.game_win = game_win;
    shared.lines_win = lines_win;
    
    // Call thread functions
    pthread_t display;
    pthread_create(&display, NULL, display_thread, NULL);

    
    for(int i = 0; i < LIZARD_THREADS; i++)
    {   pthread_t lizard;
        pthread_create(&lizard, NULL, lizard_thread, (void *)&shared);
    }

    pthread_t npc;
    pthread_create(&npc, NULL, npc_thread, (void *)&shared); 
 
    //  Start the proxy
    zmq_proxy (frontend, backend, NULL);

    // We never get here...
    return 0;
}