#include "ush.h"

static bool del_part(t_processes **processes, int num) {
    t_processes *j = *processes;

    while(j) { 
        if(j->next->num == num) {
            mx_del_proc(&j, 2, processes);
            return true;
        }
        j = j->next;
        if (j->next == NULL)
            break;
    }
    return false;
}

bool mx_proc_num_find(char *args, t_processes **processes) {
    char *tmp = mx_cut_str_forproc(args);

    printf("%s\n", tmp);
    int num = mx_atoi(tmp);
    t_processes *j = *processes;
    bool flag = true;

    if(j->num == num)
        mx_del_proc(processes, 1, processes);
    else if (j->num > 0 && j->next != NULL)
        flag = del_part(processes, num);
    else
        flag = false;
    mx_strdel(&tmp);
    if (flag == false)
        mx_printerror(0, "fg", args, "proc");
    return flag;
}
