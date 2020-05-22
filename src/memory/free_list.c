#include "ush.h"

void free_list(t_hst **list) {
    t_hst *tmp = *list;
    while(tmp) {
        t_hst *hren = tmp->next;
        if (tmp->data != NULL) 
            free(tmp->data);
        free(tmp);
        tmp = NULL;
        tmp = hren;
    }
}
