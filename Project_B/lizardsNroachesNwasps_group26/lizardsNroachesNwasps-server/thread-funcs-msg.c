#include "thread-funcs.h"
#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>
#include <zmq.h>
#include "../common-files/messages.pb-c.h"

void generate_r(void *responder, int success, int secret_code, int score)
{
    response_msg r;
    r.success = success;
    r.secret_code = secret_code;
    r.score = score;

    zmq_send(responder, &r, sizeof(r), 0);
}

void generate_r_npc(void *responder, int success, int secret_code, int score)
{
    ResponseMessage resp = RESPONSE_MESSAGE__INIT;

    resp.success = success;
    resp.secret_code = secret_code;
    resp.score = score;

    int resp_len = response_message__get_packed_size(&resp);
    char *resp_buf = malloc(resp_len);
    response_message__pack(&resp, resp_buf);

    printf("Sending message of length %d\n", resp_len);
    int rc = zmq_send(responder, resp_buf, resp_len, 0);
    assert(rc != -1);
}

int generate_code()
{
    // Get current timestamp
    time_t currentTime = time(NULL);

    // Generate a random number
    int randomNum = rand();

    // Combine timestamp and random number to create a unique code
    int uniqueCode = (int)currentTime ^ randomNum;

    return uniqueCode;
}