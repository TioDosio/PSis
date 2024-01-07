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
pthread_mutex_t mutex_clients;

typedef struct clients_t
{
    entity_type_t entity_type;  // Type of entity
    int code;                   // Secret code of client (same as first entity it represents)
    time_t last_received_time;  // Time of last received message
    pthread_t timeout_thread;   // Thread to check timeout
} clients_t;

typedef struct timeout_args {
    clients_t *client;
    thread_args *shared;
} timeout_args;


clients_t *client_array[MAX_CLIENTS];
int n_clients;

/**
 * @brief Create a new client and add it to the array
 *
 * @param entity_type type of client
 * @param code code of client
 * @param shared pointer to struct with shared data
 * 
 * @return pointer to the new client
 *
 */
clients_t *add_client(entity_type_t entity_type, int code, thread_args *shared);

/**
 * @brief Remove a client from the array
 *
 * @param client pointer to client
 * 
 * @return 1 if success, 0 if not found
 *
 */
int remove_client(clients_t *client);

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

/**
 * @brief Function to send display connect message
 *
 * @param responder socket to send message
 * @param shared pointer to struct with shared data
 * @param success success of operation so far
 * @param code secret code of entity
 *
 */
void send_display_connect(void *responder, thread_args *shared, int success, int code);


/**
 * @brief Function to respawn the eaten roaches
 *
 * @param roach roach to respawn after 5 seconds
 *
 */
void *respawn_thread(void *roach);

/**
 * @brief Function to receive updates from lizards threads and npc's thread and send to lizard client 
 *
 * @param roach roach to respawn after 5 seconds
 *
 */
void *display_thread(void *args);

/**
 * @brief The function checks if clients have not transmitted any data in the past minute. If not disconnects
 *
 * @param args pointer to struct with arguments for the thread (shared and client)
 *
 */
void *timeout_thread(void *args);

/**
 * @brief Update last used time of client and return time difference
 * @param client pointer to client
 *
 * @return time difference
 *
 */
time_t update_time(clients_t *client);

/**
 * @brief Finds a client pointer with the given secret code.
 *
 * This function searches for a client in the system with the specified secret code.
 * The secret code of a client is the same as the secret code of the FIRST entity it represents.
 *
 * @param secret_code The secret code of the client to find.
 * @return The client pointer if found, or NULL if not found.
 */
clients_t * find_client(int secret_code);

/**
 * @brief This function is responsible for checking the timeout of a thread.
 *
 * @param arg The argument passed to the thread function.
 * @return void* The return value of the thread function.
 */
void* timeout_check_thread(void* arg);


#endif