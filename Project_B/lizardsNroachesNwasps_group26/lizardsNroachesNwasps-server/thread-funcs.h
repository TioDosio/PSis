#ifndef HEADER_FILE_THREAD_FUNCS_H
#define HEADER_FILE_THREAD_FUNCS_H
#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>
#include <pthread.h>
#include <zmq.h>
#include <assert.h>
#include "entity_behaviour.h"
#include <stdlib.h>

// Mutex for protecting the shared content
pthread_mutex_t mutex_lizard;
pthread_mutex_t mutex_npc;

/**
 * @brief Thread function for handeling lizard messages and execute actions
 *
 * @param lizard_args pointer to struct with arguments for the thread
 *
 */
void *lizard_thread(void *lizard_args);

/**
 * @brief Thread function for handeling NPC messages and execute actions
 *
 * @param lizard_args pointer to struct with arguments for the thread
 *
 */
void *npc_thread(void *lizard_args);

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

/**
 * @brief Function to generate and send response to npc's client
 *
 * @param responder socket to send response
 * @param success success of operation
 * @param secret_code secret code of entity
 * @param score score of entity (if applicable)
 *
 */
void generate_r_npc(void *responder, int success, int secret_code, int score);

/**
 * @brief Updates the display with the current state of the shared data.
 *
 * This function is responsible for updating the display with the current state of the shared data.
 * It takes a pointer to a thread_args structure as a parameter.
 *
 * @param shared A pointer to a thread_args structure containing the shared data.
 */

void disp_update(thread_args *shared);

/**
 * @brief Function to generate a random code
 *
 * @return int random code
 *
 */
int generate_code();

#endif