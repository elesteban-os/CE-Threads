#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "CE_mutex.h"  

#define NUM_THREADS 5
#define NUM_INCREMENTS 100000

int shared_variable = 0;
CEmutex lock;  // Usamos CEmutex en lugar de pthread_mutex_t

void* thread_function(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        CEmutex_lock(&lock);     // Implementacion de CEmutex_lock
        shared_variable++;       
        CEmutex_unlock(&lock);   // Implementacion de CEmutex_unlock
    }
    return NULL;
}

/*
 * Este codigo fue tomado del profesor Leonardo Araya, del curso "Principios de Sistemas Operativos",
 * con el fin de validar la reimplementación(CE_mutex) de las funciones mutex de PThreads.
 * 
 * Fuente original: https://github.com/leoam/operating_systems/blob/main/processes_and_threads/example_5.c
 * 
 * Se han realizado modificaciones para integrar la implementacion propia del mutex.
 */
int main() {
    pthread_t threads[NUM_THREADS];

    
    // Inicializamos el mutex con CEmutex_init
    if (CEmutex_init(&lock) != 0) {
        perror("CEmutex_init");
        return 1;
    }

    for (long i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(&threads[i], NULL, thread_function, NULL);
        if (result != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruirmos el mutex usando CEmutex_destroy
    CEmutex_destroy(&lock);

    printf("Final value of shared variable: %d\n", shared_variable);

    return 0;
}

// Compilar con: gcc -o mutex_test mutex_test.c CE_mutex.c -lpthread