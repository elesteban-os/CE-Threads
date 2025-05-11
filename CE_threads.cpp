#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "CE_threads.h"

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
    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    thread->tid = clone(thread_func, // function to be executed
                        static_cast<char *>(thread->stack) + STACK_SIZE, // pointer to the end of the stack
                        flags,  // flag to indicate the system that needs to send a sigal when the thread finish
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

/*
 * Wrapper para la llamada al sistema futex.
 * 
 * La syscall permite que los hilos esperen o se despierten en función del estado de la variable 'lock'.
 * 
 * En caso de que el parámetro 'op' sea FUTEX_WAIT:
 *      El hilo se bloqueará (dormirá) si el valor en 'uaddr' coincide con 'val'.
 *      Utilizado cuando un hilo intenta adquirir un bloqueo pero encuentra que ya está ocupado.
 * 
 * En caso de que el parámetro 'op' sea FUTEX_WAKE:
 *      Despierta a uno o más hilos que están esperando en la palabra futex en 'uaddr'.
 *      Utilizado cuando un hilo libera un bloqueo notificando a otros hilos.
 */
int futex(int *uaddr, int op, int val, const struct timespec *timeout)
{
    return syscall(SYS_futex, uaddr, op, val, timeout, NULL, 0); // Llamada al sistema futex
}

// Funciones del mutex

/*
 * CEmutex_init: Inicializa el mutex
 * 
 * El mutex se inicializa en un estado desbloqueado, significa que la variable lock es 0.
 */
int CEmutex_init(CEmutex *mutex)
{
    mutex->lock = 0;  // Establece la variable lock en 0 (desbloqueado)
    return 0; 
}


/*
 * CEmutex_lock: Bloquea el mutex estableciendo la variable lock en 1.
 * 
 * La función utiliza la operación atómica (__sync_lock_test_and_set) para intentar adquirir
 * el bloqueo. Donde:
 * 
 * 1. Si el bloqueo ya está activo (lock = 1), el hilo esperará utilizando la llamada al sistema futex 
 *    hasta que se le notifique que el bloqueo ha sido liberado.
 * 
 * 2. Si el bloqueo no está activo (lock = 0), el hilo lo adquirirá inmediatamente.
 */
int CEmutex_lock(CEmutex *mutex)
{
    while (__sync_lock_test_and_set(&mutex->lock, 1)) // Intenta adquirir el bloqueo usando __sync_lock_test_and_set
    {
        futex(&mutex->lock, FUTEX_WAIT, 1, NULL); // Si el bloqueo ya está ocupado, espera usando futex.
    }
    return 0;
}


/*
 * CEmutex_unlock: Desbloquea el mutex estableciendo la variable lock en 0.
 * 
 * Para desbloquear el mutex, la función utiliza la operación atómica (__sync_lock_release) para liberar el bloqueo. 
 * 
 * Establece la variable lock en 0 (desbloqueado) usando mutex->lock, 
 * lo que libera el mutex y notifica a los hilos en espera mediante la llamada al sistema "futex".
 */
int CEmutex_unlock(CEmutex *mutex)
{
    __sync_lock_release(&mutex->lock);  // Libera el bloqueo automáticamente
    futex(&mutex->lock, FUTEX_WAKE, 1, NULL);   // Despierta a un hilo que esté esperando en el bloqueo
    return 0;
}


/*
 * CEmutex_destroy: Simula la destrucción del mutex
 * 
 * Nota: En esta implementación, no necesitamos hacer nada especial para destruir el mutex.
 */
int CEmutex_destroy(CEmutex *mutex)
{
    return 0; // Nada que limpiar en esta implementación.
}