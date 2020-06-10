#include "ush.h"

static void plus_on_min(t_processes **p, char a) {
    t_processes *proc = *p;
    int ind = -1;

    if (a == '+') {
        for ( ; proc; proc=proc->next) {
            if (proc->sign == '-') {
                proc->sign = '+';
                ind = proc->index - 1;
            }
            if (proc->index == ind)
                proc->sign = '-';
        }
    }
    if (a == '-') {
        for ( ; proc; proc=proc->next) {
            if (proc->sign == '+') {
                ind = proc->index - 1;
            }
            if (proc->index == ind)
                proc->sign = '-';
        }
    }
}

static void mx_reload(pid_t pid, char **args, t_processes **processes) {
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

static void del_proc (t_processes **processes, t_processes **first) {
        t_processes *j = *processes;
        t_processes *del = j->next;
        char sign = '\0';
        char **data = mx_copy_dub_arr(del->data);
        int pid = del->pid;

        j->next = NULL;
        if (del->next != NULL)
            j->next = del->next;
        del->next = NULL;
        sign = del->sign;
        mx_free_processes(&del);
        plus_on_min(processes, sign);
        mx_reload(pid, data, first);
}

static void forever_alone(t_processes **processes, t_processes **f, char **data) {
    t_processes *j = *processes;
    char sign = '\0';
    int pid = j->pid;

    if (j->data != NULL)
        mx_del_strarr(&j->data);
    mx_strdel(&j->pwd);
    j->data = NULL;
    j->num = -1;
    j->pid = -1;
    j->index = 0;
    sign = j->sign;
    j->sign = '\0';
    plus_on_min(processes, sign);
    mx_reload(pid, data, f);
}


void mx_del_proc(t_processes **processes, int flag, t_processes **first) {
    t_processes *j = *processes;
    t_processes *del = j->next;
    char **data = NULL;
    int pid = j->pid;
    char sign = '\0';

    if (flag == 1) {
        data = mx_copy_dub_arr(j->data);
        if (j->next == NULL) //когда один остается
            forever_alone(processes, first, data);
        else {
            j->next = NULL;
            sign = j->sign;
            mx_printchar(sign);
            mx_free_processes(&j);
            *processes = del;
            plus_on_min(processes, sign);
            mx_reload(pid, data, first);
        }
    }
    if (flag == 2) {
        del_proc(processes, first);
    }
}
