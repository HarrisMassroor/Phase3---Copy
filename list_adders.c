#include "list_internal.h"

static NODE *allocate_node(void *item)
{
    int i;

    for (i = 0; i < MAX_NODES; ++i) {
        if (!node_pool[i].used) {
            node_pool[i].item = item;
            node_pool[i].prev = NULL;
            node_pool[i].next = NULL;
            node_pool[i].used = 1;
            return &node_pool[i];
        }
    }
    return NULL;
}

int ListAdd(LIST *list, void *item)
{
    NODE *node;

    if (!valid_list(list) || !(node = allocate_node(item)))
        return -1;

    if (!list->head) {
        list->head = list->tail = node;
    } else if (!list->curr) {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    } else {
        node->prev = list->curr;
        node->next = list->curr->next;
        if (list->curr->next)
            list->curr->next->prev = node;
        else
            list->tail = node;
        list->curr->next = node;
    }

    list->curr = node;
    ++list->count;
    return 0;
}

int ListInsert(LIST *list, void *item)
{
    NODE *node;

    if (!valid_list(list) || !(node = allocate_node(item)))
        return -1;

    if (!list->head) {
        list->head = list->tail = node;
    } else if (!list->curr) {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    } else {
        node->next = list->curr;
        node->prev = list->curr->prev;
        if (list->curr->prev)
            list->curr->prev->next = node;
        else
            list->head = node;
        list->curr->prev = node;
    }

    list->curr = node;
    ++list->count;
    return 0;
}

int ListAppend(LIST *list, void *item)
{
    NODE *node;

    if (!valid_list(list) || !(node = allocate_node(item)))
        return -1;

    node->prev = list->tail;
    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->curr = node;
    ++list->count;
    return 0;
}

int ListPrepend(LIST *list, void *item)
{
    NODE *node;

    if (!valid_list(list) || !(node = allocate_node(item)))
        return -1;

    node->next = list->head;
    if (list->head)
        list->head->prev = node;
    else
        list->tail = node;

    list->head = node;
    list->curr = node;
    ++list->count;
    return 0;
}
