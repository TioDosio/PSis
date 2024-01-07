#include "thread-funcs.h"
#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>
#include <zmq.h>
#include <string.h>
#include "../common-files/messages.pb-c.h"

void generate_r(void *responder, int success, int secret_code, int score)
{
    response_msg r;
    r.success = success;
    r.secret_code = secret_code;
    r.score = score;

    zmq_send(responder, &r, sizeof(r), 0);
}

void generate_r_npc(void *responder, int success, int secret_code, int score)
{
    ResponseMessage resp = RESPONSE_MESSAGE__INIT;

    resp.success = success;
    resp.secret_code = secret_code;
    resp.score = score;

    int resp_len = response_message__get_packed_size(&resp);
    char *resp_buf = malloc(resp_len);
    response_message__pack(&resp, resp_buf);

    int rc = zmq_send(responder, resp_buf, resp_len, 0);
    assert(rc != -1);
}

int generate_code()
{
    // Get current timestamp
    time_t currentTime = time(NULL);

    // Generate a random number
    int randomNum = rand();

    // Combine timestamp and random number to create a unique code
    int uniqueCode = (int)currentTime ^ randomNum;

    return uniqueCode;
}

void send_display_connect(void *responder, thread_args *shared, int success, int code)
{

    // Create message
    connect_display_resp m = {0};

    if (success == 0)
    {
        m.secret_code = -1;
        
        // Send message right away, no need to read.
        zmq_send(responder, &m, sizeof(m), 0);
        return;
    } //Otherwise, read shared data and send it.

    m.secret_code = code;
    m.success = success;

    // Critical section
    pthread_mutex_lock(&mutex_lizard);
    pthread_mutex_lock(&mutex_npc);

    m.n_lizards = shared->n_lizards;
    m.n_npc = shared->n_npc;
    for (int i = 0; i < shared->n_lizards; i++)
    {
        m.lizard[i] = shared->lizard_array[i];
    }
    for (int i = 0; i < shared->n_npc; i++)
    {
        m.npc[i] = shared->npc_array[i];
    }
    strcpy(m.address_port, ADDRESS_PUB);

    // End critical section
    pthread_mutex_unlock(&mutex_lizard);
    pthread_mutex_unlock(&mutex_npc);

    // Send message
    zmq_send(responder, &m, sizeof(m), 0);
}

clients_t *add_client(entity_type_t entity_type, int secret_code){
    
    clients_t *new_client = malloc(sizeof(clients_t));
    new_client->entity_type = entity_type;
    new_client->last_received_time = time(NULL);
    new_client->code = secret_code;

    pthread_mutex_lock(&mutex_clients);
    client_array[n_clients] = new_client;
    n_clients++;
    pthread_mutex_unlock(&mutex_clients);
    

    // Create timeout_check thread
    pthread_mutex_lock(&mutex_clients);
    pthread_create(&new_client->timeout_thread, NULL, timeout_check_thread, (void*) new_client);
    pthread_mutex_unlock(&mutex_clients);
    return new_client;
}

int remove_client(clients_t *client){
    pthread_mutex_lock(&mutex_clients);
    int client_id = -1;
    for (int i = 0; i < n_clients; i++)
    {
        if (client_array[i] == client)
        {
            client_id = i;
            break;
        }
    }

    if (client_id == -1)
    {
        pthread_mutex_unlock(&mutex_clients);
        return 0;
    }

    for (int i = client_id; i < n_clients - 1; i++)
    {
        client_array[i] = client_array[i + 1];
    }
    n_clients--;
    free(client);
    pthread_mutex_unlock(&mutex_clients);
    return 1;
}

time_t update_time(clients_t *client)
{
    time_t current_time = time(NULL);
    time_t dif;

    pthread_mutex_lock(&mutex_clients);
    dif = difftime(current_time, client->last_received_time);
    client->last_received_time = current_time;
    pthread_mutex_unlock(&mutex_clients);
    return dif;
}

clients_t* find_client(int secret_code)
{
    for (int i = 0; i < n_clients; i++)
    {
        if (client_array[i]->code == secret_code)
        {
            pthread_mutex_unlock(&mutex_clients);
            return client_array[i];
        }
    }
    return NULL;
}