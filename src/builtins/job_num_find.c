#include "ush.h"

static bool del_part(t_jobs **jobs, int num) {
    t_jobs *j = *jobs;

    while(j) { 
        if(j->next->num == num) {
            del_job(&j, 2);
            return true;
        }
        j = j->next;
        if (j->next == NULL)
            break;
    }
    return false;
}

bool job_num_find(char *args, t_jobs **jobs) {
    char *tmp = cut_str_forjob(args);
    int num = mx_atoi(tmp);
    t_jobs *j = *jobs;
    bool flag = true;

    if(j->num == num)
        del_job(jobs, 1);
    else if (j->num > 0 && j->next != NULL) //остальное
        flag = del_part(jobs, num);
    else 
        flag = false;
    mx_strdel(&tmp);
    if (flag == false)
        mx_printerror(0, "fg", args, "job");
    return flag;
}
