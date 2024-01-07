#include "../common-files/lizardsNroachesNwasps.h"
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
#include "../common-files/messages.pb-c.h"

ClientMessage msg = CLIENT_MESSAGE__INIT;
ResponseMessage *resp;

client_msg m;
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
 * @return client_id that is used to send to the server do identify the client
 *
 */
int connect_roaches(int n, int *roach_codes)
{
    zmq_msg_t zmq_msg;
    zmq_msg_init(&zmq_msg);
    int rc;
    int client_id = -1;
    for (int i = 0; i < n; i++)
    {
        int points_roach = (rand() % 5) + 1;

        // Set and send message
        msg.msg_type = CONNECT;
        msg.entity_type = ROACH;
        msg.content = points_roach + '0';
        msg.client_code = client_id; 
        printf("points_roach: %d\n", points_roach);

        int msg_len = client_message__get_packed_size(&msg);
        char *msg_buf = malloc(msg_len);
        client_message__pack(&msg, msg_buf);

        rc = zmq_send(requester, msg_buf, msg_len, 0);
        assert(rc != -1);

        // Wait for response
        printf("Waiting for response\n");
        int resp_len = zmq_recvmsg(requester, &zmq_msg, 0);

        void *resp_data = zmq_msg_data(&zmq_msg);
        resp = response_message__unpack(NULL, resp_len, resp_data);
        r.score = resp->score;
        r.success = resp->success;
        r.secret_code = resp->secret_code;
        
        response_message__free_unpacked(resp, NULL);

        if (client_id == -1){
            client_id = r.secret_code;
        }
        if (r.success == 0)
        {
            printf("Server Full, try again later\n");
            break;
        }
        roach_codes[i] = r.secret_code;
        free(msg_buf);
    }
    return client_id;
}

int main(int argc, char *argv[])
{
    zmq_msg_t zmq_msg;
    zmq_msg_init(&zmq_msg);
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
    printf("Server: %s\n", server_address);
    assert(rc == 0);

    // Send n_roaches connection messages
    msg.client_code = connect_roaches(n_roaches, roach_codes);

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
            m.content = direction;
            m.secret_code = roach_codes[i];

            msg.msg_type = m.msg_type;
            msg.entity_type = m.entity_type;
            msg.content = m.content;
            msg.secret_code = m.secret_code;
            int msg_len = client_message__get_packed_size(&msg);
            char *msg_buf = malloc(msg_len);
            client_message__pack(&msg, msg_buf);

            rc = zmq_send(requester, msg_buf, msg_len, 0);
            assert(rc != -1);

            int resp_len = zmq_recvmsg(requester, &zmq_msg, 0);

            void *resp_data = zmq_msg_data(&zmq_msg);
            resp = response_message__unpack(NULL, resp_len, resp_data);
            r.score = resp->score;
            r.success = resp->success;
            r.secret_code = resp->secret_code;
            response_message__free_unpacked(resp, NULL);

            if(r.success == 0){
                sleep(1);
                printf("Operation failed. Timed out?\n");
                sleep(1);
                continue;
            }
        }
    }
    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;
}
