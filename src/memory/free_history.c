#include "ush.h"

void free_history(t_history **list) {
    t_history *tmp = *list;
    while(tmp) {
        t_history *history = tmp->next;
        if (tmp->data != NULL) 
            free(tmp->data);
        free(tmp);
        tmp = NULL;
        tmp = history;
    }
}
