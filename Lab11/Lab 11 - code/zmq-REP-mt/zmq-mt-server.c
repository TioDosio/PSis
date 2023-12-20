#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

#define LENGTH_ARRAY 1000

int total_primes = 0;
int prime_array[LENGTH_ARRAY];

// Mutex for protecting the shared counter
pthread_mutex_t counter_mutex;

int verify_prime( int value){

    int divisor = 2;
    if(value < 4){
        return 1;
    }
    while (value%divisor != 0){
        divisor ++;
    }
    if (value == divisor){
        return 1;
    }else{
        return 0;
    }
}

void *  context;

void *thread_print(void *ptr){
    
        long int thread_number = (long int)ptr;
    
        // Socket to get results
        void *puller = zmq_socket (context, ZMQ_PULL);
        int rc = zmq_bind (puller, "inproc://xxxx");
        assert (rc == 0);
    
        int n;
        printf("Thread ready\n");
        while (1) {
            zmq_recv(puller, &n, sizeof(n), 0);
            printf ("PRIME FOUND: Thread %ld received %d\n", thread_number, n);
        }
        return 0;
    
    
}

void *thread_function( void *ptr ){

    long int thread_number = (long int)ptr;

    // Socket to reply to clients
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_connect (responder, "inproc://back-end");
    assert (rc == 0);

    // Socket to share results
    void *pusher = zmq_socket (context, ZMQ_PUSH);
    rc = zmq_connect (pusher, "inproc://xxxx");
    assert (rc == 0);

    int n;
    int r;
    printf("Thread ready\n");
    while (1) {
        
        zmq_recv(responder, &n, sizeof(n), 0);
        printf ("Thread %ld received %d\n", thread_number, n);
        
        r = verify_prime(n);

        if(r == 1){
            // Lock the mutex before accessing the shared resource
            pthread_mutex_lock(&counter_mutex);

            // Critical section
            prime_array[total_primes] = n;
            total_primes++;

            // Unlock the mutex after accessing the shared resource
            pthread_mutex_unlock(&counter_mutex);

            // Send the result to the pusher
            zmq_send(pusher, &n, sizeof(n), 0);
        }

        zmq_send(responder, &r, sizeof(r), 0);
    }
    return 0;

}


int main (void)
{

    context = zmq_ctx_new ();

    //  Socket facing clients
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    int rc = zmq_bind (frontend, "ipc:///tmp/fifo-pipe-front-end");
    assert (rc == 0);

    //  Socket facing services
    void *backend = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend, "inproc://back-end");
    assert (rc == 0);



    
    long int worker_nbr;
    for (worker_nbr = 0; worker_nbr < 2; worker_nbr++) {
        pthread_t worker;
        pthread_create( &worker, NULL, thread_function, (void *) worker_nbr);
    }

    pthread_t printer;
    pthread_create( &printer, NULL, thread_print, (void *) worker_nbr);


    //  Start the proxy
    zmq_proxy (frontend, backend, NULL);

    //  We never get here...
    return 0;
}