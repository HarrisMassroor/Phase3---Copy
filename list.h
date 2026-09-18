#ifndef LIST_H
#define LIST_H

#define MAX_LISTS 32
#define MAX_NODES 1024

typedef struct node NODE;
typedef struct list LIST;

LIST *ListCreate(void);
int ListCount(LIST *list);
void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);
int ListAdd(LIST *list, void *item);
int ListInsert(LIST *list, void *item);
int ListAppend(LIST *list, void *item);
int ListPrepend(LIST *list, void *item);
void *ListRemove(LIST *list);
void *ListTrim(LIST *list);
void ListConcat(LIST *first, LIST *second);
void ListFree(LIST *list, void (*itemFree)(void *));
void *ListSearch(LIST *list, int (*comparator)(void *, void *),
                 void *comparisonArg);

#endif
