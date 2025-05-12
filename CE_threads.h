#ifndef CETHREADS_H
#define CETHREADS_H

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sched.h>
#include <cstdlib>
#include <cstdio>
#include <semaphore.h>
#include <iostream>


#define STACK_SIZE 1024 * 1024  // 1 MB per thread

typedef struct {
    void *stack;
    pid_t tid;
    sem_t finished;  // semaphore to synchronize
} CEthread_t;

typedef struct {
    int lock;
} CEmutex;

int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg);
int CEthread_join(CEthread_t *thread);

int CEmutex_init(CEmutex *mutex);       // Inicializa el mutex en un estado desbloqueado.
int CEmutex_lock(CEmutex *mutex);       // Bloquea el mutex, esperando si ya está bloqueado.
int CEmutex_unlock(CEmutex *mutex);     // Desbloquea el mutex y despierta a los hilos en espera.
int CEmutex_destroy(CEmutex *mutex);    // Destruye el mutex (sin efecto en esta implementación).

#endif // CETHREADS_H