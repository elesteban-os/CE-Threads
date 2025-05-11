#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "CE_mutex.hpp"  

#define NUM_THREADS 5
#define NUM_INCREMENTS 100000

using namespace std;

int shared_variable = 0;
CEmutex lock;  // Usamos CEmutex en lugar de pthread_mutex_t

void* thread_function(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        CEmutex_lock(&lock);     // Usar tu función lock
        shared_variable++;       // Incrementar la variable compartida
        CEmutex_unlock(&lock);   // Usar tu función unlock
    }
    return nullptr;
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

    // Imprimir el valor final de la variable compartida
    cout << "Valor final de la variable compartida: " << shared_variable << endl;

    return 0;
}

// Compilar con: g++ -o mutex_test mutex_test.cpp CE_mutex.cpp -lpthread