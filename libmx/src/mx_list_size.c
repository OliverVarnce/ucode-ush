#include "libmx.h"

int mx_list_size(t_list *list) {
    int size = 0;

    for (t_list *walker = list; walker != NULL; walker = walker->next, size++);

    return size;
}
