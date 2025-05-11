// g++ main.cpp CE_threads.cpp -o programa -Wall

#include <stdio.h>
#include <unistd.h>
#include "CE_threads.h"

void *task1(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 5; i++) {
        printf("Hilo %d (task1): iteración %d\n", id, i);
        sleep(1);
    }
    return NULL;
}

void *task2(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 3; i++) {
        printf("Hilo %d (task2): paso %d\n", id, i);
        sleep(1);
    }
    return NULL;
}

int main() {
    CEthread_t thread1, thread2;

    int id1 = 1;
    int id2 = 2;

    if (CEthread_create(&thread1, task1, &id1) != 0) {
        perror("Fallo al crear thread1");
        return 1;
    }

    if (CEthread_create(&thread2, task2, &id2) != 0) {
        perror("Fallo al crear thread2");
        return 1;
    }

    if (CEthread_join(thread1) != 0) {
        perror("Fallo al esperar thread1");
    }

    if (CEthread_join(thread2) != 0) {
        perror("Fallo al esperar thread2");
    }

    printf("Todos los hilos han terminado.\n");
    return 0;
}
