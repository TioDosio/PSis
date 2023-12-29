#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>

// Mutex for protecting the shared content
pthread_mutex_t mutex_lizard;
pthread_mutex_t mutex_npc;

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