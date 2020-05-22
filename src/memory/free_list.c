#include "ush.h"

void free_list(t_history **list) {
    t_history *tmp = *list;
    while(tmp) {
        t_history *hren = tmp->next;
        if (tmp->data != NULL) 
            free(tmp->data);
        free(tmp);
        tmp = NULL;
        tmp = hren;
    }
}
