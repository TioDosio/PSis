#include "lizardsNroaches.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <zmq.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char server_address[256];
    char *server_ip = "127.0.0.1";
    char *server_port = "6666";
    // para colocar o ip e a porta como argumentos
    if (argc != 3)
    {
        printf("You insert %d arguments, you need 3\n", argc);
    }
    else
    {
        server_ip = argv[1];
        server_port = argv[2];
    }

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    snprintf(server_address, sizeof(server_address), "tcp://%s:%s", server_ip, server_port);
    zmq_connect(requester, server_address);
    response_msg r;
    srand(time(NULL));
    int points_roach = (rand() % 4) + 1;

    generic_msg m;
    m.msg_type = 0;
    m.entity_type = ROACH;
    m.ch = points_roach + '0';
    zmq_send(requester, &m, sizeof(m), 0);
    zmq_recv(requester, &r, sizeof(r), 0);
    printf("Received %d, secrect: %d\n", r.success, r.secrect_code);
    if (r.success == 0)
    {
        printf("Server Full, try again later\n");
        exit(0);
    }
    m.secrect_code = r.secrect_code;
    int sleep_delay;
    direction_t direction;
    int n = 0;
    while (1)
    {
        n++;
        sleep_delay = random() % 700000;
        usleep(sleep_delay);
        direction = random() % 4;
        switch (direction)
        {
        case LEFT:
            printf("%d Going Left   \n", n);
            break;
        case RIGHT:
            printf("%d Going Right   \n", n);
            break;
        case DOWN:
            printf("%d Going Down   \n", n);
            break;
        case UP:
            printf("%d Going Up    \n", n);
            break;
        }

        m.direction = direction;
        m.msg_type = 1;
        zmq_send(requester, &m, sizeof(m), 0);
        zmq_recv(requester, &r, sizeof(r), 0);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}
