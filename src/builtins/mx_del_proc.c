#include "ush.h"

void mx_reload(pid_t pid, char **args, t_processes **processes) {
    int status;
    pid_t tmp;
    pid_t wait;
    
    tmp = fork();
    if (tmp == 0) {
        kill(pid, SIGCONT);
    }
    else {
        wait = waitpid(tmp, &status, WUNTRACED);
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
            if (WIFSTOPPED(status)) {
                mx_add_proc(processes, args, pid);
                break;
            }
            wait = waitpid(tmp, &status, WUNTRACED);
        }
    }
}

static void del_proc(t_processes **processes) {
    t_processes *j = *processes;
    t_processes *del = j->next;
    
    mx_reload(del->pid, del->data, processes);
    j->next = NULL;
    j->next = del->next;
    del->next = NULL;
    free_processes(&del);
}


void mx_del_proc(t_processes **processes, int flag) {
    t_processes *j = *processes;
    t_processes *del = j->next;
    
    if (flag == 1) { //голова
        //kill(j->pid, SIGCONT);
        mx_reload(j->pid, j->data, processes);
        if (j->next == NULL) { //когда один остается
            if (j->data != NULL) 
                mx_del_strarr(&j->data);
            mx_strdel(&j->pwd);
            j->data = NULL;
            j->num = -1;
            j->pid = -1;
            return ;
        }
        j->next = NULL;
        free_processes(&j);
        *processes = del;
    }
    if (flag == 2) { //тело
        del_proc(processes);
    }
}
