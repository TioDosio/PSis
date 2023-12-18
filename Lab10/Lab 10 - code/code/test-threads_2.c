#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_function(void *arg)
{
    int n = 0;
    long int r = random() % 100;
    printf("Start Thread %lu\n", r);
    int banana = *(int *)arg;
    while (n < 10)
    {
        sleep(banana);
        n++;
        printf("inside Thread %lu %d\n", r, n);
    }
    printf("End Thread %lu\n", r);
    return (void *)r;
}

int main()
{
    char line[100];
    int n_threads;
    printf("How many threads: ");
    fgets(line, 100, stdin);
    sscanf(line, "%d", &n_threads);
    pthread_t thread_id;
    pthread_t *thread_id_array = malloc(sizeof(pthread_t) * n_threads);

    void *reto_value;
    int i = 0;
    int *banana = malloc(sizeof(int));
    *banana = 1;
    while (i < n_threads)
    {
        pthread_create(&thread_id_array[i], NULL, thread_function, banana);
        i++;
    };
    for (i = 0; i < n_threads; i++)
    {
        pthread_join(thread_id_array[i], &reto_value);
        int ret = (int)reto_value;
        printf("Thread %d returned %d\n", i, ret);
    }

    printf("Carregue em enter para terminar\n");
    free(thread_id_array);
    free(banana);
    exit(0);
}