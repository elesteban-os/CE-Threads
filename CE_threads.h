#ifndef CETHREADS_H
#define CETHREADS_H

#include <sys/types.h>

#define STACK_SIZE 1024 * 1024  // 1 MB per thread

typedef struct {
    pid_t tid;
    void *stack;
} CEthread_t;

int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg);
int CEthread_join(CEthread_t thread);

#endif // CETHREADS_H