#include "lizardsNroaches.h"
#include <zmq.h>

int main()
{

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, ADDRESS_RC);
}