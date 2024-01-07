#include <stdio.h>
#include <unistd.h>
#include "../common-files/lizardsNroachesNwasps.h"
#include "thread-funcs.h"
#include "../common-files/display-funcs.h"
#include "../common-files/messages.pb-c.h"
#include <zmq.h>
#define DELTA 1

// Function to check and disconnect if no messages received for a minute
void* timeout_check_thread(void* arg) {
    
    //Load variables
    clients_t *client = (clients_t *)arg;
    int done = 1;

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
    zmq_msg_t zmq_msg;
    zmq_msg_init(&zmq_msg);
    ClientMessage msg = CLIENT_MESSAGE__INIT;
    ResponseMessage *resp;

    
    response_msg r;
    client_msg m;
    m.msg_type = DISCONNECT;
    m.content = 0;

    pthread_mutex_lock(&mutex_clients);
    m.secret_code = client->code;
    m.entity_type = client->entity_type;
    pthread_mutex_unlock(&mutex_clients);
    code_list_t *npc_list;
    

    while (1) {
        // Sleep for a short duration to avoid continuous checking
        sleep(DELTA);

        // Get the current time
        time_t current_time = time(NULL);

        // Check the time difference since the last received message for this client
        int time_difference = difftime(current_time, client->last_received_time);

        // If no messages received for a minute, disconnect the client
        if (time_difference >= TIME_OUT) {
            
            switch (client->entity_type){
            
                case LIZARD:
                    // Send disconnect message to display
                    rc = zmq_send(requester, &m, sizeof(m), 0);
                    assert(rc != -1);
                    rc = zmq_recv(requester, &r, sizeof(r), 0);
                    assert(rc != -1);
                    break;
                case ROACH:
                case WASP:
                    done = 1;
                    pthread_mutex_lock(&mutex_clients);
                    npc_list = client->npc_list;
                    for(code_list_t *npc = npc_list; npc != NULL; npc = npc->next)
                    {
                        // Set and send message
                        msg.msg_type = DISCONNECT;
                        msg.entity_type = client->entity_type;
                        msg.content = 0;
                        msg.client_code = client->code; 
                        msg.secret_code = npc->code;
                        

                        int msg_len = client_message__get_packed_size(&msg);
                        char *msg_buf = malloc(msg_len);
                        client_message__pack(&msg, msg_buf);

                        rc = zmq_send(requester, msg_buf, msg_len, 0);
                        assert(rc != -1);

                        // Wait for response
                        int resp_len = zmq_recvmsg(requester, &zmq_msg, 0);

                        void *resp_data = zmq_msg_data(&zmq_msg);
                        resp = response_message__unpack(NULL, resp_len, resp_data);
                        r.success = resp->success;
                        
                        response_message__free_unpacked(resp, NULL);

                        free(msg_buf);
                        if (r.success == 0) done = 0;
                    }
                    pthread_mutex_unlock(&mutex_clients);
                    
                    break;                
            }

            if (done) 
            {
                remove_client(client);
                zmq_close(requester);
                pthread_exit(NULL); 
            }

        }
    }
}