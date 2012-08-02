#ifndef __TS_QUEUE_H
#define __TS_QUEUE_H

#include <semaphore.h>
#include "list.h"

struct ts_queue {
    sem_t           avail;
    pthread_mutex_t lock;
    struct list     lst;
};

struct ts_queue * queue_init();
void queue_free(struct ts_queue * q);
void * queue_pop(struct ts_queue * q);
void queue_push(struct ts_queue * q, void * elem);

#endif
