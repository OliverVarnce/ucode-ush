#include "ush.h"
static void to_negative(t_processes **j, char p) {
    t_processes *proc = *j;
    int neg = -1;

    if (p == '+') {
        for ( ; proc; proc=proc->next) {
            if (proc->sign == '-') {
                proc->sign = '+';
                neg = proc->index - 1;
            }
            if (proc->index == neg)
                proc->sign = '-';
        }
    }
    if (p == '-') {
        for ( ; proc; proc = proc->next) {
            if (proc->sign == '+') {
                neg = proc->index - 1;
            }
            if (proc->index == neg)
                proc->sign = '-';
        }
    }
}

void reload(pid_t pid, char **args, t_processes **processes) {
    int status;

    tcsetpgrp(0, pid);
    tcsetpgrp(1, pid);
    kill(-pid, SIGCONT);
    waitpid(-pid, &status, WUNTRACED);
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        mx_add_proc(processes, args, pid);
    }
    if (args != NULL)
        mx_del_strarr(&args);
    errno = 0;
}

static void del_body(t_processes **processes) {
    t_processes *j = *processes;
    t_processes *del = j->next;
    
    reload(del->pid, del->data, processes);
    j->next = NULL;
    j->next = del->next;
    del->next = NULL;
    mx_free_processes(&del);
}


void mx_del_proc(t_processes **processes, int flag) {
    t_processes *j = *processes;
    t_processes *del = j->next;
    char **data = NULL;
    int pid = j->pid;
    char sign = '\0';

    if (flag == 1) {
        data = mx_copy_dub_arr(j->data);
        if (j->next == NULL) {
            if (j->data != NULL)
                mx_del_strarr(&j->data);
            mx_strdel(&j->pwd);
            j->data = NULL;
            j->num = -1;
            j->pid = -1;
            j->index = 0;
            sign = j->sign;
            j->sign = '\0';
            to_negative(processes, sign);
            reload(pid, data, &j);
        }
        else {
            j->next = NULL;
            sign = j->sign;
            mx_printchar(sign);
            mx_free_processes(&j);
            *processes = del;
            to_negative(processes, sign);
            reload(pid, data, processes);
        }
    }
    if (flag == 2) {
        del_body(processes);
    }

}
