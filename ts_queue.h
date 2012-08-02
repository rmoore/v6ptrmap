#ifndef __TS_QUEUE_H
#define __TS_QUEUE_H

#include <semaphore.h>
#include "list.h"

struct ts_queue {
    sem_t           avail;
    pthread_mutex_t lock;
    int             count;

    /* Accesses to the following are mediated by the lock mutex */
    struct list     lst;
    int             waiting;
};

struct ts_queue * queue_init(int count);
void queue_free(struct ts_queue * q);
void * queue_pop(struct ts_queue * q);
void queue_push(struct ts_queue * q, void * elem);
int  queue_empty(struct ts_queue * q);

#endif
