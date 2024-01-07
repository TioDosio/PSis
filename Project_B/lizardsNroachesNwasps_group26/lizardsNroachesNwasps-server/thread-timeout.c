#include <stdio.h>
#include <unistd.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include "thread-funcs.h"
#include "../common-files/display-funcs.h"
#define DELTA 1

// Function to check and disconnect if no messages received for a minute
void* timeout_check_thread(void* arg) {
    
    //Load variables
    clients_t *client = (clients_t *)arg;

    // Initialize ZMQ
    void *requester = zmq_socket(context, ZMQ_REQ);
    int rc;
    switch (client->entity_type){
            
                case LIZARD:
                    rc = zmq_connect(requester, ADDRESS_REQ_LIZ);
                    break;
                case ROACH:
                case WASP:
                   rc = zmq_connect(requester, ADDRESS_REQ_NPC);
                    break;                
            }
    assert(rc != -1);
    
    response_msg r;
    client_msg m;
    m.msg_type = DISCONNECT;
    m.content = 0;

    pthread_mutex_lock(&mutex_clients);
    m.secret_code = client->code;
    m.entity_type = client->entity_type;
    pthread_mutex_unlock(&mutex_clients);

    while (1) {
        // Sleep for a short duration to avoid continuous checking
        sleep(DELTA);

        // Get the current time
        time_t current_time = time(NULL);

        // Check the time difference since the last received message for this client
        int time_difference = difftime(current_time, client->last_received_time);

        // If no messages received for a minute, disconnect the client
        if (time_difference >= TIME_OUT) {
            // Send disconnect message to display
            rc = zmq_send(requester, &m, sizeof(m), 0);
            assert(rc != -1);
            rc = zmq_recv(requester, &r, sizeof(r), 0);
            assert(rc != -1);
            if (r.success == 1)
                zmq_close(requester);
                pthread_exit(NULL);
        }
    }
}