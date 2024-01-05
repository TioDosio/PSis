#include "thread-funcs.h"
#include "../common-files/messages.pb-c.h"
#include "../common-files/lizardsNroachesNwasps.h"
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
    void *publisher = zmq_socket(context, ZMQ_PUB); // Create socket for PUB-SUB
    // rc = zmq_bind(publisher, ADDRESS_PUB);          // Bind to address
    assert(rc == 0);

    // Initialize variables
    client_msg m;
    int code;
    display_update update;
    int success;

    zmq_msg_t zmq_msg;
    zmq_msg_init(&zmq_msg);
    ClientMessage *msg;

    // loop for handling messages
    while (1)
    {
        update.disconnect = 0;
        update.id_l_bumped = -1;
        success = 0;
        // Set success to 0

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
            generate_r(responder, 0, -1, 0);
            continue;
        }
        printf("Sending response %d\n", success);
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
            printf("Sending response %d\n", success);
            generate_r(responder, success, code, 0);

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
                    shared->npc_array[i].secret_code = -1;
                    shared->n_npc--;

                    // success
                    success = 1;
                    update.disconnect = 1;
                    break;
                }
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_npc);

            // Send response
            generate_r(responder, success, m.secret_code, 0);

            break;

        case MOVE:
            move_npc(m.secret_code, m.content, shared);

            // Send response
            generate_r(responder, success, m.secret_code, 0);
            break;
        }
        // update.entity = shared->npc_array[];
        //  Update display
        disp_update(shared);
        // Send display update to lizard-clients
        // zmq_send(publisher, &update, sizeof(update), 0);
    }
}
