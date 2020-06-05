#include "ush.h"

static void to_body(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;
    
    while (proc) {
        if (proc->next == NULL) {
            proc->next = mx_create_proc(args, proc->num + 1,
                pid, getenv("PWD"));
            break;
        }
        else if (proc->next->num == proc->num + 1) //до последней
            proc = proc->next;
        else { //пропущеный номер
            t_processes *tmp = proc->next;
            proc->next = mx_create_proc(args, proc->num + 1,
                pid, getenv("PWD"));
            proc->next->next = tmp;
            break;
        }  
    }
}

void add_proc(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;

    if (proc->data == NULL && proc->num == -1) { //если пустой
        proc->data = mx_copy_dub_arr(args);
        proc->num = 1;
        proc->pid = pid;
        proc->pwd = mx_strdup(getenv("PWD"));
        return ;
    }
    else if (proc->num != 1) {
        t_processes *tmp = mx_create_proc(args, 1, pid, getenv("PWD"));
        tmp->next = proc;
        *j = tmp;
        return ;
    }
    to_body(j, args, pid);
}
