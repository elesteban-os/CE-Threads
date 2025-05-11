#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "CE_threads.h"  

#define NUM_THREADS 5
#define NUM_INCREMENTS 100

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
    CEthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];  // Arreglo para almacenar los argumentos de cada hilo

    // Inicializamos el mutex con CEmutex_init
    if (CEmutex_init(&lock) != 0) {
        perror("CEmutex_init");
        return 1;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;  // Asignar un valor único para cada hilo
        int result = CEthread_create(&threads[i], thread_function, &thread_args[i]);
        if (result != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        CEthread_join(threads[i]);
    }

    // Destruimos el mutex usando CEmutex_destroy
    CEmutex_destroy(&lock);

    // Imprimir el valor final de la variable compartida
    cout << "Valor final de la variable compartida: " << shared_variable << endl;

    return 0;
}

// Compilar con: g++ -o mutex_test mutex_test.cpp CE_mutex.cpp -lpthread