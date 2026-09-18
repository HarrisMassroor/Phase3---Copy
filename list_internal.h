#ifndef LIST_INTERNAL_H
#define LIST_INTERNAL_H

#include <stddef.h>
#include "list.h"

struct node {
    void *item;
    NODE *prev;
    NODE *next;
    int used;
};

struct list {
    NODE *head;
    NODE *tail;
    NODE *curr;
    int count;
    int used;
};

extern LIST list_pool[MAX_LISTS];
extern NODE node_pool[MAX_NODES];

#define valid_list(list) ((list) != NULL && (list)->used)

#endif
