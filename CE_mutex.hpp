#ifndef CEMUTEX_H
#define CEMUTEX_H

#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

/*
 * CEmutex: Esta estructura representa un mutex
 * 
 * El mutex está representado por una única variable entera 'lock'.
 * 
 * 0 = desbloqueado
 * 1 = bloqueado
 */
typedef struct {
    int lock;
} CEmutex;

int CEmutex_init(CEmutex *mutex);       // Inicializa el mutex en un estado desbloqueado.
int CEmutex_lock(CEmutex *mutex);       // Bloquea el mutex, esperando si ya está bloqueado.
int CEmutex_unlock(CEmutex *mutex);     // Desbloquea el mutex y despierta a los hilos en espera.
int CEmutex_destroy(CEmutex *mutex);    // Destruye el mutex (sin efecto en esta implementación).


#endif // CEMUTEX_H

