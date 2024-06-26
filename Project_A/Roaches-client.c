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
#include <assert.h>

generic_msg m;
response_msg r;

void *context;
void *requester;
char server_address[256];

/*
 * @brief spawn n roaches
 *
 * @param n number of roaches to spawn
 * @param roach_codes array to store the secret codes of the roaches
 *
 */
void spawn_roaches(int n, int *roach_codes)
{
    int rc;
    for (int i = 0; i < n; i++)
    {
        int points_roach = (rand() % 5) + 1;

        // Set and send message
        m.msg_type = 0;
        m.entity_type = ROACH;
        m.ch = points_roach + '0';

        rc = zmq_send(requester, &m, sizeof(m), 0);
        assert(rc != -1);

        // Wait for response
        rc = zmq_recv(requester, &r, sizeof(r), 0);
        assert(rc != -1);

        if (r.success == 0)
        {
            printf("Server Full, try again later\n");
            break;
        }
        roach_codes[i] = r.secret_code;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    char *server_ip;
    char *server_port;
    int n_roaches;

    switch (argc)
    {
    case 3:
        server_ip = argv[1];
        server_port = argv[2];
        n_roaches = rand() % MAX_ROACH_PER_CLIENT + 1;
        break;
    case 4:
        server_ip = argv[1];
        server_port = argv[2];
        if (sscanf(argv[3], "%d", &n_roaches) == 1)
        {
            if (n_roaches > MAX_ROACH_PER_CLIENT)
            {
                printf("Max cockroaches per client is %d\n", MAX_ROACH_PER_CLIENT);
                return 1;
            }
            else if (n_roaches < 1)
            {
                printf("The number of cockroaches need to be positive\n");
                return 1;
            }
        }
        else
        {
            printf("Invalid input for the number os cockroaches\n");
            return 1;
        }
        break;
    default:
        printf("Usage: %s <server_ip> <server_port> [n_roaches]\n", argv[0]);
        return 1;
    }
    // Check if the user provided the correct number of command-line arguments

    int roach_codes[MAX_ROACH_PER_CLIENT];

    int sleep_delay;
    direction_t direction;
    int n = 0;
    int rc;
    // creating request socket
    printf("Connecting to server…\n");
    context = zmq_ctx_new();
    requester = zmq_socket(context, ZMQ_REQ);
    snprintf(server_address, sizeof(server_address), "tcp://%s:%s", server_ip, server_port);
    rc = zmq_connect(requester, server_address);
    assert(rc != -1);

    // Send n_roaches connection messages
    spawn_roaches(n_roaches, roach_codes);

    while (1)
    {
        m.entity_type = ROACH;
        m.msg_type = 1;
        for (int i = 0; i < n_roaches; i++)
        {
            sleep_delay = random() % 600000;
            usleep(sleep_delay);
            direction = random() % 4;

            // Generate random direction
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

            // Set and send message
            m.direction = direction;
            m.secret_code = roach_codes[i];
            rc = zmq_send(requester, &m, sizeof(m), 0);
            assert(rc != -1);

            rc = zmq_recv(requester, &r, sizeof(r), 0);
            assert(rc != -1);
        }
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}
