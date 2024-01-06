#include "../common-files/lizardsNroachesNwasps.h"
#include "thread-funcs.h"
#include <string.h>

void *display_thread(void *args)
{   
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, ADDRESS_PUB);
    assert(rc == 0);

    //  Socket to commmunicate with other threads (liz and npc)
    void *sock_recv = zmq_socket (context, ZMQ_PULL);
    rc = zmq_bind (sock_recv, ADDRESS_PULL);
    assert (rc == 0);

    display_update update;
    char *string = "dis";
    
    while (1) {
        zmq_recv(sock_recv, &update, sizeof(update), 0);

        if (zmq_send(publisher, string, strlen(string), ZMQ_SNDMORE) == -1)
        {
            continue;
        }
        if (zmq_send(publisher, &update, sizeof(update), 0) == -1)
        {
            continue;
        }
    }
    
}