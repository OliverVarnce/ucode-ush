#include "ush.h"

static void to_body(t_jobs **j, char **args, pid_t pid) {
    t_jobs *job = *j;
    
    while (job) {
        if (job->next == NULL) {
            job->next = mx_create_job(args, job->num + 1, 
                pid, getenv("PWD"));
            break;
        }
        else if (job->next->num == job->num + 1) //до последней
            job = job->next;
        else { //пропущеный номер
            t_jobs *tmp = job->next;
            job->next = mx_create_job(args, job->num + 1, 
                pid, getenv("PWD"));
            job->next->next = tmp;
            break;
        }  
    }
}

void add_job(t_jobs **j, char **args, pid_t pid) {
    t_jobs *job = *j;

    if (job->data == NULL && job->num == -1) { //если пустой
        job->data = copy_dub_arr(args);
        job->num = 1;
        job->pid = pid;
        job->pwd = mx_strdup(getenv("PWD"));
        return ;
    }
    else if (job->num != 1) {
        t_jobs *tmp = mx_create_job(args, 1, pid, getenv("PWD"));
        tmp->next = job;
        *j = tmp;
        return ;
    }
    to_body(j, args, pid);
}
