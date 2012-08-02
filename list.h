#ifndef __LIST_H
#define __LIST_H

struct link {
    struct link * next;
    struct link * prev;
    void * elem;
};

struct list {
    struct link * first;
    struct link * last;
};

void * list_pop(struct list * l);
void   list_push(struct list * l, void * e);
void   list_init(struct list * l);
void   list_free(struct list * l); /* Assumes an empty list */
int    list_empty(struct list * l);

#endif
