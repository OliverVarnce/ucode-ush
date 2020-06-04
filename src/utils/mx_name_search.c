#include "ush.h"

int mx_name_search(char *tmp , t_processes *processes) {
    int res = -1;
    int count = 0;
    t_processes *j = processes;
    if (j->data != NULL) {
        while(j) {
            if (mx_get_substr_index(j->data[0], tmp) == 0)
                res = count;
            j = j->next;
            count++;
        }
    }
    return res;
}
