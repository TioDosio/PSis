#include "thread-funcs.h"
#include "../common-files/lizardsNroachesNwasps.h"
#include <pthread.h>
#include <zmq.h>

void generate_r(void *responder, int success, int secret_code, int score)
{
    response_msg r;
    r.success = success;
    r.secret_code = secret_code;
    r.score = score;

    zmq_send(responder, &r, sizeof(r), 0);
}