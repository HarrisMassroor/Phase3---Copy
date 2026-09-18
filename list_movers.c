#include "list_internal.h"

int ListCount(LIST *list)
{
    return valid_list(list) ? list->count : 0;
}

void *ListFirst(LIST *list)
{
    if (!valid_list(list) || !list->head)
        return NULL;
    list->curr = list->head;
    return list->curr->item;
}

void *ListLast(LIST *list)
{
    if (!valid_list(list) || !list->tail)
        return NULL;
    list->curr = list->tail;
    return list->curr->item;
}

void *ListNext(LIST *list)
{
    if (!valid_list(list) || !list->curr)
        return NULL;
    list->curr = list->curr->next;
    return list->curr ? list->curr->item : NULL;
}

void *ListPrev(LIST *list)
{
    if (!valid_list(list) || !list->curr)
        return NULL;
    list->curr = list->curr->prev;
    return list->curr ? list->curr->item : NULL;
}

void *ListCurr(LIST *list)
{
    return valid_list(list) && list->curr ? list->curr->item : NULL;
}
