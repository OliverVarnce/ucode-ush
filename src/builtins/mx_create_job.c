#include "ush.h"

t_jobs *mx_create_job(char **data, int num, pid_t pid, char *pwd)
{
    t_jobs *t = (t_jobs *)malloc(sizeof(t_jobs) * 4);
    t->data = copy_dub_arr(data);
    t->pwd = cut_str_forjob(pwd);
    t->num = num;
    t->pid = pid;
    t->next = NULL;
    return t;
}
