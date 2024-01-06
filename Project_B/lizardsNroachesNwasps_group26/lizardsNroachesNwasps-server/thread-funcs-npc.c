#include "thread-funcs.h"
#include "../common-files/messages.pb-c.h"
#include "../common-files/lizardsNroachesNwasps.h"
#include "../common-files/display-funcs.h"
#include <string.h>

void *npc_thread(void *npc_args)
{
    // Load shared variables
    thread_args *shared = (thread_args *)npc_args;

    // Initialize ZMQ
    // Socket to reply to clients
    void *responder = zmq_socket(context, ZMQ_REP); // Create socket por REQ-REP
    int rc = zmq_bind(responder, ADDRESS_REQ_NPC);  // Bind to address
    assert(rc == 0);

    // Socket to send display updates
    void *pusher = zmq_socket(context, ZMQ_PUSH); // Create socket for PUB-SUB
    rc = zmq_connect(pusher, ADDRESS_PULL);       // Connect to address
    assert(rc == 0);

    // Initialize variables
    client_msg m;
    int code;
    display_update update;
    int success;
    int points;
    int disc;
    int id_npc;

    zmq_msg_t zmq_msg;
    zmq_msg_init(&zmq_msg);
    ClientMessage *msg;

    // loop for handling messages
    while (1)
    {
        // Set returns to default value
        disc = 0;
        success = 0;

        // Receive message

        int msg_len = zmq_recvmsg(responder, &zmq_msg, 0);

        void *msg_data = zmq_msg_data(&zmq_msg);
        msg = client_message__unpack(NULL, msg_len, msg_data);
        m.content = msg->content;
        m.entity_type = msg->entity_type;
        m.msg_type = msg->msg_type;
        m.secret_code = msg->secret_code;
        client_message__free_unpacked(msg, NULL);
        if (m.entity_type == LIZARD)
        {
            // Send error message
            generate_r_npc(responder, 0, -1, 0);
            continue;
        }
        switch (m.msg_type)
        {
        case CONNECT:

            // CRITICAL SECTION
            pthread_mutex_lock(&mutex_npc);

            // Check if there is room for more npc's
            if (shared->n_npc < MAX_NPCS)
            {
                // Add the npc to array
                code = spawn_entity(shared, m.entity_type);

                // success
                success = 1;
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_npc);

            // Send response [HERE] MUST SEND DISPLAY_CONNECT?
            generate_r_npc(responder, success, code, 0);

            break;

        case DISCONNECT:
            // CRITICAL SECTION
            pthread_mutex_lock(&mutex_npc);

            // Check if npc exists
            for (int i = 0; i < shared->n_npc; i++)
            {
                if (shared->npc_array[i].secret_code == m.secret_code)
                {
                    // Remove npc from array
                    remove_entity(shared->npc_array, &shared->n_npc, i);

                    // success
                    success = 1;
                    disc = 1;
                    break;
                }
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_npc);

            // Send response
            generate_r_npc(responder, success, m.secret_code, 0);

            break;

        case MOVE:
            success = move_npc(m.secret_code, m.content, shared, &id_npc);

            // Send response
            generate_r_npc(responder, success, m.secret_code, 0);
            break;
        }
        // Create update struct (used to send display updates)
        pthread_mutex_lock(&mutex_lizard);
        pthread_mutex_lock(&mutex_npc);
        update.entity = shared->npc_array[id_npc];
        pthread_mutex_unlock(&mutex_lizard);
        pthread_mutex_unlock(&mutex_npc);
        update.id_l_bumped = -1;
        update.disconnect = disc;

        //  Update display (critical section)
        pthread_mutex_lock(&mutex_lizard);
        pthread_mutex_lock(&mutex_npc);
        disp_update(shared);
        pthread_mutex_unlock(&mutex_lizard);
        pthread_mutex_unlock(&mutex_npc);

        // Send display update to display-thread
        zmq_send(pusher, &update, sizeof(update), 0);
    }
}
