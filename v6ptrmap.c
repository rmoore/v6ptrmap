/*
 * Copyright (c) 2012 Reese Moore <ram@vt.edu>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include "ts_queue.h"

#define THREADS 8

void * worker(void * arg);
void worker_function(struct ts_queue * queue, struct ts_queue * success);
void push_all_prefixes(struct ts_queue * q, char * addr);
int try_resolv(char * addr);
void usage(char * name);

struct args {
    struct ts_queue * success;
    struct ts_queue * queue;
};

int 
main(int argc, char **argv)
{
    int i;
    pthread_t t[THREADS];


    /* Make sure there is one argument */
    if (argc == 1) {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* Initialize The Queues */
    struct ts_queue * success = queue_init(0); 
    struct ts_queue * queue = queue_init(THREADS);

    /* Seed the queue with the initial value */
    for (i = 1; i < argc; i++) {
        queue_push(queue, strdup(argv[i]));
    }

    /* Build the arguments structure */
    struct args * args = (struct args *) malloc(sizeof(struct args));
    args->success = success;
    args->queue = queue;

    /* Start some workers */
    for (i = 0; i < THREADS; i++) {
        pthread_create(&t[i], NULL, worker, (void *)args); 
    }
    
    /* Try to print the from the success queue forever */
    while (1) {
        char * addr = queue_pop(success);
        printf("%s\n", addr);
        free(addr);
    }

    return EXIT_SUCCESS;
}

void *
worker(void * arg)
{
    struct args * args = (struct args *) arg;

    while (1) {
        worker_function(args->queue, args->success);
    }
    return NULL;
}

void
worker_function(struct ts_queue * queue, struct ts_queue * success)
{

    char * addr = queue_pop(queue);
    int res = try_resolv(addr);
    if (res == 0) {
        /* Found an address */
        queue_push(success, strdup(addr));
    } else if (res == 4) {
        /* Found a lead */
        push_all_prefixes(queue, addr);
    }
    free(addr);

    /* Anything else indicates that there aren't any more addresses down that
     * path. Usually we would get a code of 1 (NXDOMAIN) for this, but anything
     * really indicates that
     */
    return;
}

void 
push_all_prefixes(struct ts_queue * q, char * addr)
{
    static char * hex_fmt[] = {"0.%s", "1.%s", "2.%s", "3.%s", "4.%s", "5.%s",
                               "6.%s", "7.%s", "8.%s", "9.%s", "A.%s", "B.%s",
                               "C.%s", "D.%s", "E.%s", "F.%s"};
    int i;
    for(i = 0; i < 16; i++) {
        char * new_addr;
        if (asprintf(&new_addr, hex_fmt[i], addr) < 0) abort();
        queue_push(q, new_addr);
    }
}

int
try_resolv(char * addr)
{
    unsigned char ans[256];
    int len = res_query(addr, C_IN, T_PTR, ans, 256);
    if (len < 0) {
        return h_errno;
    } else {
        return 0;
    }
}

void
usage(char * name)
{
    printf("Usage:\n\t%s <partial .ip6.arpa record>...\n", name);
}
