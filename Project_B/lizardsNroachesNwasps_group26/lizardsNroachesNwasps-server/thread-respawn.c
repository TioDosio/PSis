#include <stdio.h>
#include <unistd.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include "thread-funcs.h"

void *respawn_thread(void *args){

    entity_t *roach = (entity_t *)args;
    sleep(5);

    if (roach->secret_code != -1 && roach->entity_type == 1){
        //LOCK
        pthread_mutex_lock(&mutex_npc);
        roach->ch = ((rand() % 5) + 1) + '0';
        roach->pos_x = (rand() % (WINDOW_SIZE - 2)) + 1;
        roach->pos_y = (rand() % (WINDOW_SIZE - 2)) + 1;
        //UNLOCK
        pthread_mutex_unlock(&mutex_npc);
    }
}
