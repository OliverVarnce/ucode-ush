#include "ush.h"

void mx_free_processes(t_processes **processes) {
    t_processes *j = *processes;
    while(j) {
        t_processes *proc = j->next;
        if (j->data != NULL) 
            mx_del_strarr(&j->data);
        mx_strdel(&j->pwd);
        j->data = NULL;
        j->num = 0;
        j->pid = 0;

        free(j);
        j = NULL;
        j = proc;
    }
}
