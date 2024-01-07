#include <stdio.h>
#include <unistd.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include "thread-funcs.h"
#include "../common-files/display-funcs.h"
#define DELTA 2

// Function to check and disconnect if no messages received for a minute
void* timeout_check_thread(void* arg) {
    
    //Load variables
    timeout_args *args = (timeout_args *)arg;
    thread_args *shared = args->shared;
    clients_t *client = args->client;

    //Free the memory allocated for the arguments
    free(args);

    while (1) {
        // Sleep for a short duration to avoid continuous checking
        sleep(DELTA);

        // Get the current time
        time_t current_time = time(NULL);

        // Check the time difference since the last received message for this client
        int time_difference = difftime(current_time, client->last_received_time);

        // If no messages received for a minute, disconnect the client
        if (time_difference >= TIME_OUT) {
            // Disconnect the client
            switch (client->entity_type){
            
                case LIZARD:
                    pthread_mutex_lock(&mutex_lizard);
                    pthread_mutex_lock(&mutex_clients);
                    for (int i=0; i < MAX_LIZARDS; i++) {
                        if (client->code == shared->lizard_array[i].secret_code) {
                            remove_entity(shared->lizard_array, &shared->n_lizards, i);
                            break;
                        }
                    }
                    pthread_mutex_unlock(&mutex_lizard);
                    pthread_mutex_unlock(&mutex_clients);
                    break;
                case ROACH:
                case WASP:
                    pthread_mutex_lock(&mutex_npc);
                    pthread_mutex_lock(&mutex_clients);
                    for (int i=0; i < MAX_NPCS; i++) {
                        if (client->code == shared->npc_array[i].secret_code) {
                            remove_entity(shared->npc_array, &shared->n_npc, i);
                            break;
                        }
                    }

                    pthread_mutex_unlock(&mutex_npc);
                    pthread_mutex_unlock(&mutex_clients);
                    break;

                remove_client(client);
            }
            pthread_exit(NULL);
        }
    }
}