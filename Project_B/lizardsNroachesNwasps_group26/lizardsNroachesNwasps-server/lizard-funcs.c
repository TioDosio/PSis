#include "lizard-funcs.h"
#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>
#include <zmq.h>

void *lizard_thread(void *lizard_args)
{
    // Load shared variables
    thread_args *shared = (thread_args *)lizard_args;
    
    // Initialize ZMQ 
    void *responder = zmq_socket(context, ZMQ_REP); // Create socket por REQ-REP
    //void *publisher = zmq_socket(context, ZMQ_PUB); // Create socket for PUB-SUB

    int rc = zmq_connect(responder, BACK_END_ADDRESS); // Bind to address
    assert(rc == 0);

    // rc = zmq_bind(publisher, ADDRESS_PUB); // Bind to address
    // assert(rc == 0);

    // Initialize variables
    client_msg m;
    int code;
    int success;

    // loop for handling messages
    while(1)
    {
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
                // Generate Secrete code
                code = generate_code();

                //CRITICAL SECTION
                pthread_mutex_lock(&mutex);

                // Check if there is room for more lizards
                if (shared->n_lizards < MAX_LIZARDS)
                {
                    // Add lizard to array 
                    shared->lizard_array[shared->n_lizards].secret_code = code;
                    shared->lizard_array[shared->n_lizards].points = 0;
                    shared->n_lizards++;

                    // success
                    success = 1;

                }

                //END CRITICAL SECTION
                pthread_mutex_unlock(&mutex);
                
                // Send response
                generate_r(responder, success, code, 0);
                 
                break;

            case DISCONNECT:
                //CRITICAL SECTION
                pthread_mutex_lock(&mutex);

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
                        break;
                    }
                }

                //END CRITICAL SECTION
                pthread_mutex_unlock(&mutex);

                // Send response
                generate_r(responder, success, m.secret_code, 0);

                break;

            case MOVE: //TO DO

        }

    }
}

void generate_r(void *responder, int success, int secret_code, int score)
{
    response_msg r;
    r.success = success;
    r.secret_code = secret_code;
    r.score = score;

    zmq_send(responder, &r, sizeof(r), 0);
}