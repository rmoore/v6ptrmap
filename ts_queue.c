#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "ts_queue.h"
#include "list.h"

struct ts_queue *
queue_init()
{
    struct ts_queue * q = (struct ts_queue *) malloc(sizeof(struct ts_queue));

    sem_init(&q->avail, 0, 0);
    pthread_mutex_init(&q->lock, NULL);
    list_init(&q->lst);

    return q;
}

void
queue_free(struct ts_queue * q)
{
    list_free(&q->lst);
    pthread_mutex_destroy(&q->lock);
    sem_destroy(&q->avail);
    free(q);
}

void *
queue_pop(struct ts_queue * q)
{
    void * elem;

    /* Wait for something to arrive in the queue. */
    sem_wait(&q->avail);

    /* Pull something off the queue */
    pthread_mutex_lock(&q->lock);
    elem = list_pop(&q->lst);
    pthread_mutex_unlock(&q->lock);

    /* Return the element */
    return elem;
}

void
queue_push(struct ts_queue * q, void * elem)
{
    /* Put something on the queue */
    pthread_mutex_lock(&q->lock);
    list_push(&q->lst, elem);
    pthread_mutex_unlock(&q->lock);

    /* Post that there is something on the list */
    sem_post(&q->avail);
}
