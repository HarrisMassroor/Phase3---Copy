#include "list_internal.h"

static void release_node(NODE *node)
{
    node->item = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->used = 0;
}

void *ListRemove(LIST *list)
{
    NODE *node;
    void *item;

    if (!valid_list(list) || !(node = list->curr))
        return NULL;
    item = node->item;
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;
    list->curr = node->next;
    --list->count;
    release_node(node);
    return item;
}

void *ListTrim(LIST *list)
{
    NODE *node;
    void *item;

    if (!valid_list(list) || !(node = list->tail))
        return NULL;
    item = node->item;
    list->tail = node->prev;
    if (list->tail)
        list->tail->next = NULL;
    else
        list->head = NULL;
    list->curr = list->tail;
    --list->count;
    release_node(node);
    return item;
}

void ListConcat(LIST *first, LIST *second)
{
    if (!valid_list(first) || !valid_list(second) || first == second)
        return;
    if (second->head) {
        if (first->tail) {
            first->tail->next = second->head;
            second->head->prev = first->tail;
        } else {
            first->head = second->head;
        }
        first->tail = second->tail;
        first->count += second->count;
    }
    second->head = NULL;
    second->tail = NULL;
    second->curr = NULL;
    second->count = 0;
    second->used = 0;
}

void ListFree(LIST *list, void (*itemFree)(void *))
{
    NODE *node;
    NODE *next;

    if (!valid_list(list))
        return;
    for (node = list->head; node; node = next) {
        next = node->next;
        if (itemFree)
            itemFree(node->item);
        release_node(node);
    }
    list->head = NULL;
    list->tail = NULL;
    list->curr = NULL;
    list->count = 0;
    list->used = 0;
}

void *ListSearch(LIST *list, int (*comparator)(void *, void *),
                 void *comparisonArg)
{
    NODE *node;

    if (!valid_list(list) || !comparator)
        return NULL;
    for (node = list->curr; node; node = node->next) {
        list->curr = node;
        if (comparator(node->item, comparisonArg))
            return node->item;
    }
    list->curr = NULL;
    return NULL;
}
