#include "ush.h"

t_processes *mx_create_proc(char **data, int num, pid_t pid, char *pwd)
{
    t_processes *t = (t_processes *)malloc(sizeof(t_processes) * 4);
    t->data = mx_copy_dub_arr(data);
    t->pwd = cut_str_forproc(pwd);
    t->num = num;
    t->pid = pid;
    t->next = NULL;
    return t;
}
