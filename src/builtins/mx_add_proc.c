#include "ush.h"

static void make_negaitve(t_processes **j, int num) {
    t_processes *proc = *j;

    for ( ; proc; proc = proc->next) {
        if (proc->index != num && proc->sign == '+')
            proc->sign = '-';
        else if (proc->index != num && proc->sign == '-')
            proc->sign = '\0';
    }
}

static int find_bigest(t_processes *j) {
    int res = 0;

    for (; j; j=j->next) {
        if (j->index > res)
            res = j->index;
    }
    return res + 1;
}

static void insert(t_processes **j, pid_t pid, char **args, t_processes **f) {
    t_processes *proc = *j;
    t_processes *tmp = proc->next;

    proc->next = mx_create_proc(args, proc->num + 1,
                              pid, getenv("PWD"));
    proc->next->next = tmp;
    proc->next->index = find_bigest(proc);
    proc->next->sign = '+';
    make_negaitve(f, proc->next->index);
    return;
}

static void to_proc(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;
    
    while (proc) {
        if (proc->next == NULL) {
            proc->next = mx_create_proc(args, proc->num + 1,
                pid, getenv("PWD"));
            proc->next->index = find_bigest(proc);
            proc->next->sign = '+';
            make_negaitve(j, proc->next->index);
            break;
        }
        else if (proc->next->num == proc->num + 1)
            proc = proc->next;
        else {
            insert(&proc, pid, args, j);
            break;
        }  
    }
}

void mx_add_proc(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;

    if (proc->data == NULL && proc->num == -1) {
        proc->data = mx_copy_dub_arr(args);
        proc->num = 1;
        proc->pid = pid;
        proc->pwd = mx_strdup(getenv("PWD"));
        return;
    }
    else if (proc->num != 1) {
        t_processes *tmp = mx_create_proc(args, 1, pid, getenv("PWD"));
        tmp->next = proc;
        *j = tmp;
        return ;
    }
    to_proc(j, args, pid);
}
