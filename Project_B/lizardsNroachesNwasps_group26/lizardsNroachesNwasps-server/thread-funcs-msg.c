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