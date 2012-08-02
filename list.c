#include <stdlib.h>
#include "list.h"

void
list_init(struct list * lst)
{
    struct link * first = (struct link *) malloc(sizeof(struct link));
    struct link * last = (struct link *) malloc(sizeof(struct link));
    
    lst->first = first;
    first->next = last;
    last->prev = first;
    lst->last = last;
}

void
list_free(struct list * l)
{
    free(l->last);
    free(l->first);
    free(l);
}

void
list_push(struct list * l, void * e)
{
    struct link * lnk = (struct link *) malloc(sizeof(struct link));
    lnk->elem = e;

    l->first->next->prev = lnk;
    lnk->next = l->first->next;
    lnk->prev = l->first;
    l->first->next = lnk;
}

void *
list_pop(struct list * l)
{
    void * e = l->last->prev->elem;
    struct link * lst = l->last->prev;

    l->last->prev->prev->next = l->last;
    l->last->prev = lst->prev;

    free(lst);

    return e;
}
