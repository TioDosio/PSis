#include "thread-funcs.h"

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
    void *publisher = zmq_socket(context, ZMQ_PUB); // Create socket for PUB-SUB
    // rc = zmq_bind(publisher, ADDRESS_PUB);          // Bind to address
    assert(rc == 0);

    // Initialize variables
    client_msg m;
    int code;
    display_update update;
    int success;

    // loop for handling messages
    while (1)
    {
        update.disconnect = 0;
        update.id_l_bumped = -1;
        // Set success to 0
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
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_lizard);

            // Send response [HERE] MUST SEND DISPLAY_CONNECT?
            generate_r(responder, success, code, 0);

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
                    shared->lizard_array[i].secret_code = -1;
                    shared->n_lizards--;

                    // success
                    success = 1;
                    update.disconnect = 1;
                    break;
                }
            }

            // END CRITICAL SECTION
            pthread_mutex_unlock(&mutex_lizard);

            // Send response
            generate_r(responder, success, m.secret_code, 0);

            break;

        case MOVE:
            move_lizard(m.secret_code, m.content, shared);

            // Send response
            generate_r(responder, success, m.secret_code, 0);
            break;
        }
        // update.entity = shared->lizard_array[m.secret_code];
        //  Update display
        disp_update(shared);
        // Send display update to lizard-clients
        // zmq_send(publisher, &update, sizeof(update), 0);
    }
}
