#include "thread-funcs.h"
#include "../common-files/display-funcs.h"

void *lizard_thread(void *lizard_args)
{
    // Load shared variables
    thread_args *shared = (thread_args *)lizard_args;

    // Initialize ZMQ
    // Socket to reply to clients
    void *responder = zmq_socket(context, ZMQ_REP);    // Create socket por REQ-REP
    int rc = zmq_connect(responder, BACK_END_ADDRESS); // Bind to address
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
    int id_l_bumped;
    int l_id;

    // loop for handling messages
    while (1)
    {
        // Set returns to default value
        disc = 0;
        id_l_bumped = -1;
        success = 0;

        // Receive message
        zmq_recv(responder, &m, sizeof(m), 0);

        // Check if message is NOT from a lizard
        if (m.entity_type != LIZARD)
        {
            // Send error message
            generate_r(responder, 0, -1, 0);
            continue;
        }

        switch (m.msg_type)
        {
        case CONNECT:
            // CRITICAL SECTION
            pthread_mutex_lock(&mutex_lizard);

            // Check if there is room for more lizards
            if (shared->n_lizards < MAX_LIZARDS)
            {
                // Add lizard to array
                code = spawn_entity(shared, LIZARD);

                // success
                success = 1;
                //Save lizard id
                l_id = shared->n_lizards - 1;
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_lizard);

            // Send response (in this case, its display connect, not r)
            send_display_connect(responder, shared, success, code);

            break;

        case DISCONNECT:
            // CRITICAL SECTION
            pthread_mutex_lock(&mutex_lizard);

            // Check if lizard exists
            for (int i = 0; i < shared->n_lizards; i++)
            {
                if (shared->lizard_array[i].secret_code == m.secret_code)
                {
                    // Remove lizard from array
                    remove_entity(shared->lizard_array, &shared->n_lizards, i);
    void *context = zmq_ctx_new();
                    // success
                    success = 1;
                    disc = 1;
                    break;
                }
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_lizard);

            // Send response
            generate_r(responder, success, m.secret_code, 0);

            break;

        case MOVE:
            success = move_lizard(m.secret_code, m.content, shared, &points, &id_l_bumped, &l_id);

            generate_r(responder, success, m.secret_code, points);
            break;
        }
        // Create update struct (used to send display updates)
        pthread_mutex_lock(&mutex_lizard);
        pthread_mutex_lock(&mutex_npc);
        update.entity = shared->lizard_array[l_id];
        pthread_mutex_unlock(&mutex_lizard);
        pthread_mutex_unlock(&mutex_npc);
        update.id_l_bumped = id_l_bumped;
        update.disconnect = disc;

        //  Update display (critical section)
        pthread_mutex_lock(&mutex_lizard);
        pthread_mutex_lock(&mutex_npc);
        disp_update(shared);
        pthread_mutex_unlock(&mutex_lizard);
        pthread_mutex_unlock(&mutex_npc);

        // Send display update to display thread
        zmq_send(pusher, &update, sizeof(update), 0);
    }
}
