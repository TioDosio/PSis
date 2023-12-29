#include <zmq.h>
#include <ncurses.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "display-funcs.h"
#include <string.h>
#include <pthread.h>
#include "thread-funcs.h"
#include "npc-funcs.h"

time_t current_time;

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

    return 0;
}
