#include "ush.h"

void mx_free_processes(t_processes **processes) {
    t_processes *pr = *processes;
    while(pr) {
        t_processes *proc = pr->next;
        if (pr->data != NULL) 
            mx_del_strarr(&pr->data);
        mx_strdel(&pr->pwd);
        pr->data = NULL;
        pr->num = 0;
        pr->pid = 0;

        free(pr);
        pr = NULL;
        pr = proc;
    }
}
