#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>

// Struct for arguments to lizard thread
typedef struct thread_args
{
    entity_t *lizard_array;
    entity_t *npc_array;
    int n_lizards;
    int n_npc;
    int *roach_death_time;

} thread_args;

/**
 * @brief Thread function for handeling lizard messages and exacute actions
 * 
 * @param lizard_args pointer to struct with arguments for the thread
 * 
 */
void *lizard_thread(void *lizard_args);

/**
 * @brief Function to generate and send response to lizard client
 * 
 * @param responder socket to send response
 * @param success success of operation
 * @param secret_code secret code of entity
 * @param score score of entity (if applicable)
 * 
 */
void generate_r(void *responder, int success, int secret_code, int score);