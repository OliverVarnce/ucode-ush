#include "ush.h"

void reload(pid_t pid, char **args, t_jobs **jobs) {
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
                add_job(jobs, args, pid);
                break;
            }
            wait = waitpid(tmp, &status, WUNTRACED);
        }
    }
}

static void del_body(t_jobs **jobs) {
    t_jobs *j = *jobs;
    t_jobs *del = j->next;
    
    reload(del->pid, del->data, jobs);
    j->next = NULL;
    j->next = del->next;
    del->next = NULL;
    free_jobs(&del);
}


void del_job(t_jobs **jobs, int flag) {
    t_jobs *j = *jobs;
    t_jobs *del = j->next;
    
    if (flag == 1) { //голова
        //kill(j->pid, SIGCONT);
        reload(j->pid, j->data, jobs);
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
        free_jobs(&j);
        *jobs = del;
    }
    if (flag == 2) { //тело
        del_body(jobs);
    }
}
