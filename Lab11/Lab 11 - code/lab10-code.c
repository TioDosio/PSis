#define _GNU_SOURCE
#include <pthread.h>
#include<stdio.h>
#include <stdlib.h>

#define LENGTH_ARRAY 1000
int rand_num_array[LENGTH_ARRAY];
int prime_array[LENGTH_ARRAY];

int next_random_index = 0; 
int total_primes = 0;

// Mutex for protecting the shared counter
pthread_mutex_t counter_mutex;

#define N_THREADS 2
/**
 * @brief funcao que verifica se um número é primo
 * 
 * @param value valor a verificar se é primo
 * @return int 1 se value for primo 0, se não for primo
 */
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
/**
 * @brief código da thread. itera sobre o array rand_num_array e verifica se os numero são primos
 * 
 * @param arg indica da thread. É usado para cada threda saber que valores ler do array
 * @return void* numero de primos encontrados
 */
void * inc_thread(void * arg){

    int int_arg = (int) arg;
    int partial_count = 0;
    int n_primes = 0;
    int current_index = 0;
    
    

    while (next_random_index<LENGTH_ARRAY)
    {   
        // Lock the mutex before accessing the shared resource
        pthread_mutex_lock(&counter_mutex);

        //Critical section
        current_index = next_random_index;
        next_random_index++;

        // Unlock the mutex after updating the shared resource
        pthread_mutex_unlock(&counter_mutex);

        // Check if the index is out of bounds
        if(current_index >= LENGTH_ARRAY){
            break;
        }

        if(verify_prime(rand_num_array[current_index]) == 1){
            printf("\tThread %d:\t\t%d %lu is prime\n",int_arg, current_index,rand_num_array[current_index]);
            n_primes ++;
            prime_array[total_primes] = rand_num_array[current_index];
            total_primes ++;
        }else{
            printf("\tThread %d:\t\t%d %lu is not prime\n",int_arg, current_index,rand_num_array[current_index]);
        }


        partial_count++;

    }
    
    

    printf("Thread %d found %d primes on %d numbers\n", int_arg, n_primes, partial_count);
    pthread_exit(NULL);

}

/**
 * @brief programa principal que cria várias threads para verificarem se os interiso num array sao primos ou nao
 * 
 * @return int 
 */
int main(){

     // Initialize the mutex
    pthread_mutex_init(&counter_mutex, NULL);


    for(int  i= 0 ; i < LENGTH_ARRAY; i++){
        // if the following line ins uncommented there will be 59 primes in the array
        rand_num_array[i] = rand()/10; 
        // if the following line is uncommented there will be 170 prime numbers
        //rand_num_array[i] = i; 
    }

    pthread_t t_id[N_THREADS];
    for(int i = 0 ; i < 2; i++){
        pthread_create(&t_id[i], NULL, inc_thread, i);
    }


    int ret_val;
    for(int i = 0 ; i < N_THREADS; i++){
        pthread_join(t_id[i], &ret_val);
    }

    printf("main - total primes %i\n", total_primes);
    pthread_mutex_destroy(&counter_mutex);
    exit(0);
}