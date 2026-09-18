#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static int compare_ints(void *item, void *argument)
{
    return *(int *)item == *(int *)argument;
}

static void free_int(void *item)
{
    free(item);
}

static int *make_int(int value)
{
    int *item = malloc(sizeof(*item));
    if (item)
        *item = value;
    return item;
}

static void print_list(LIST *list, const char *label)
{
    int *item;

    printf("%s:", label);
    for (item = ListFirst(list); item; item = ListNext(list))
        printf(" %d", *item);
    printf(" (count=%d)\n", ListCount(list));
}

int main(void)
{
    LIST *list = ListCreate();
    LIST *other = ListCreate();
    int target = 3;
    int *found;
    int *removed;

    if (!list || !other)
        return EXIT_FAILURE;
    if (ListAppend(list, make_int(2)) != 0 ||
        ListPrepend(list, make_int(1)) != 0 ||
        ListAppend(list, make_int(4)) != 0)
        return EXIT_FAILURE;

    ListFirst(list);
    if (ListAdd(list, make_int(3)) != 0)
        return EXIT_FAILURE;
    print_list(list, "after adders");

    ListFirst(list);
    found = ListSearch(list, compare_ints, &target);
    printf("search for %d: %s\n", target, found ? "found" : "not found");

    ListFirst(list);
    removed = ListRemove(list);
    printf("removed current item: %d\n", removed ? *removed : -1);
    free(removed);
    removed = ListTrim(list);
    printf("trimmed item: %d\n", removed ? *removed : -1);
    free(removed);

    ListAppend(other, make_int(10));
    ListAppend(other, make_int(20));
    ListConcat(list, other);
    print_list(list, "after concat");

    ListFree(list, free_int);
    printf("after free: count=%d\n", ListCount(list));
    return EXIT_SUCCESS;
}
