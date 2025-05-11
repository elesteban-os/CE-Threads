#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "CE_threads.h"
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

/*
 * struct for the thread arguments 
 */
typedef struct {
    void *(*func)(void *);  // pointer to the function to be executed
    void *arg; // generic pointer (required by "clone")
} thread_args_t;

/*
 * static function defined to give it to "clone"
 */
static int thread_func(void *arg) {
    thread_args_t *args = (thread_args_t *)arg; // cast the arguments recievied to type thread_args_t
    args->func(args->arg); // execute the function referenced by the pointer recieved with the arguments
    delete args; // frees the dinamic memory used to create the thread_args_t
    return 0;
}

/*
 * CE_thread function to create threads
 */
int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg) {
    thread->stack = malloc(STACK_SIZE); // reserve dynamically memory for the stack of the thread
    if (!thread->stack) return -1; // verify if stack was reserved correctly

    thread_args_t *args = new thread_args_t; // reserve memory for the thread_args_t struct
    args->func = start_routine; // assign start_routine (the function to be executed) in the pointer func
    args->arg = arg; // assign the arguments recieved to the pointer args (this args will be passed to the function that will be executed)
    
    // invoke the system call "clone"
    thread->tid = clone(thread_func, // function to be executed
                        static_cast<char *>(thread->stack) + STACK_SIZE, // pointer to the end of the stack
                        SIGCHLD,  // flag to indicate the system that needs to send a sigal when the thread finish
                        args); // arguments
                        // if the thread had success returns the thread ID, if not, returns -1

    return (thread->tid == -1) ? -1 : 0; // compare if the execution returns -1, then returns -1, if not, return 0 (success)
}

/*
 * CE_thread function to wait the end of a thread execution and then clean resources
 */
int CEthread_join(CEthread_t thread) {
    int status; // contains the exit status of the thread
    pid_t r = waitpid(thread.tid, &status, 0); // calls waitpid to wait until the thread ends (equivalent to fork())
    std::free(thread.stack); // frees the stack
    return (r == -1) ? -1 : 0;
}