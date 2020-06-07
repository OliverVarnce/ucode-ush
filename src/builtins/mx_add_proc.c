#include "ush.h"
static void make_negative(t_processes **j, int num) {
    t_processes *proc = *j;

    for ( ; proc; proc = proc->next) {
        if (proc->index != num && proc->sign == '+')
            proc->sign = '-';
        else if (proc->index != num && proc->sign == '-')
            proc->sign = '\0';
    }
}

static void to_body(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;
    
    while (proc) {
        if (proc->next == NULL) {
            proc->next = mx_create_proc(args, proc->num + 1,
                pid, getenv("PWD"));
            break;
        }
        else if (proc->next->num == proc->num + 1)
            proc = proc->next;
        else {
            t_processes *tmp = proc->next;
            proc->next = mx_create_proc(args, proc->num + 1,
                pid, getenv("PWD"));
            proc->next->next = tmp;
            break;
        }  
    }
}

int mx_check_max(t_processes *j) {
    int res = 0;

    for (; j; j=j->next) {
        if (j->index > res)
            res = j->index;
    }
    return res + 1;
}

void mx_add_proc(t_processes **j, char **args, pid_t pid) {
    t_processes *proc = *j;

    // if (isatty(0))
    //     printf("\nush: suspended %s\n", args[0]);
    if (proc->data == NULL && proc->num == -1) {
        proc->data = mx_copy_dub_arr(args);
        proc->index = 0;
        proc->sign = '+';
        proc->num = 1;
        proc->pid = pid;
        proc->pwd = mx_strdup(getenv("PWD"));
        return;
    }
    else if (proc->num != 1) { //добавление вместо головы
        t_processes *tmp = mx_create_proc(args, 1, pid, getenv("PWD"));
        proc->index = mx_check_max(proc);
        tmp->sign = '+';
        tmp->next = proc;
        *j = tmp;
        make_negative(j, 0);
        return;
    }
    to_body(j, args, pid);
}
