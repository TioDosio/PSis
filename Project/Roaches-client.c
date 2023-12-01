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
    int port, ip;
    // para colocar o ip e a porta como argumentos
    if (argc == 3)
    {
        port = atoi(argv[2]);
        ip = atoi(argv[1]);
    }
    else
    {
        port = 5555; // mudar para o ip e porta do servidor
        ip = "";
    }

    // creating request socket
    printf("Connecting to server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, ADDRESS_RC);
    response_msg r;
    srand(time(NULL));
    int points_roach = rand() % 4 + 1;

    generic_msg m;
    m.msg_type = 0;
    m.entity_type = ROACH;
    m.ch = points_roach + '0';
    zmq_send(requester, &m, sizeof(m), 0);
    zmq_recv(requester, &r, sizeof(r), 0);
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
        printf("Received %d\n", r.success);
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}
