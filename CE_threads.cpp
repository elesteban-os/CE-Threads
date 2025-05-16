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
    CEthread_t *thread;  // a pointer to the thread struct
} thread_args_t;

/*
 * static function defined to give it to "clone"
 */
static int thread_func(void *arg) {
    if (!arg) {
        std::cout << "Error: Argumento nulo en thread_func." << std::endl;
        return -1;
    }

    thread_args_t *args = static_cast<thread_args_t *>(arg);
    args->func(args->arg);
    sem_post(&args->thread->finished);
    delete args;
    return 0;
}

/*
 * static function defined to give it to "clone"
 */
int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg) {
    if (sem_init(&thread->finished, 0, 0) != 0) {
        std::cout << "Error: No se pudo inicializar el semáforo." << std::endl;
        return -1;
    }

    thread->stack = malloc(STACK_SIZE);
    if (!thread->stack) {
        std::cout << "Error: No se pudo asignar memoria para la pila del hilo." << std::endl;
        return -1;
    }

    thread_args_t *args = new thread_args_t;
    if (!args) {
        std::cout << "Error: No se pudo asignar memoria para los argumentos del hilo." << std::endl;
        free(thread->stack);
        return -1;
    }

    args->func = start_routine;
    args->arg = arg;
    args->thread = thread;

    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD;
    thread->tid = clone(thread_func,
                        static_cast<char *>(thread->stack) + STACK_SIZE,
                        flags,
                        args);

    if (thread->tid == -1) {
        std::cout << "Error: No se pudo crear el hilo con clone." << std::endl;
        delete args;
        free(thread->stack);
        return -1;
    }

    return 0;
}

/*
 * CE_thread function to wait the end of a thread execution and then clean resources
 */
int CEthread_join(CEthread_t *thread) {
    std::cout << "antes" << std::endl;
    sem_wait(&thread->finished);  // wait until the thread finish
    std::cout << "despues" << std::endl;
    //sem_destroy(&thread->finished);  // lclean the semaphore
    //std::free(thread->stack); // frees the stack
    return 0;
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
