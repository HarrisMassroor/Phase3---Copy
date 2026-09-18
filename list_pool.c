#include "list_internal.h"

LIST list_pool[MAX_LISTS];
NODE node_pool[MAX_NODES];

LIST *ListCreate(void)
{
    int i;

    for (i = 0; i < MAX_LISTS; ++i) {
        if (!list_pool[i].used) {
            list_pool[i].head = NULL;
            list_pool[i].tail = NULL;
            list_pool[i].curr = NULL;
            list_pool[i].count = 0;
            list_pool[i].used = 1;
            return &list_pool[i];
        }
    }
    return NULL;
}
