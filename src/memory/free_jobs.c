#include "ush.h"

void free_jobs(t_jobs **jobs) {
    t_jobs *j = *jobs;
    while(j) {
        t_jobs *hren = j->next;
        if (j->data != NULL) 
            mx_del_strarr(&j->data);
        mx_strdel(&j->pwd);
        j->data = NULL;
        j->num = 0;
        j->pid = 0;

        free(j);
        j = NULL;
        j = hren;
    }
}
