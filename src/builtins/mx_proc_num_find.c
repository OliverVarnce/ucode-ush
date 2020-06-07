#include "ush.h"

static bool del_part(t_processes **processes, int num) {
    t_processes *j = *processes;

    while(j) { 
        if(j->next->num == num) {
            mx_del_proc(&j, 2);
            return true;
        }
        j = j->next;
        if (j->next == NULL)
            break;
    }
    return false;
}

bool proc_num_find(char *args, t_processes **processes) {
    char *tmp = cut_str_forproc(args);
    int num = mx_atoi(tmp);
    t_processes *j = *processes;
    bool flag = true;

    if(j->num == num)
        mx_del_proc(processes, 1);
    else if (j->num > 0 && j->next != NULL) //остальное
        flag = del_part(processes, num);
    else 
        flag = false;
    mx_strdel(&tmp);
    if (flag == false)
        mx_printerror(0, "fg", args, "proc");
    return flag;
}
