#include "ush.h"

static bool cur_proc(char *tmp , t_processes **processes){
    t_processes *j = *processes;
    
    if (mx_strcmp_null(tmp, "%") == 0 ||
        mx_strcmp_null(tmp, "+") == 0 || tmp == NULL) { 
        if (j->data == NULL && j->num == -1) {
            mx_printstr("fg: no current proc\n");
            return false;
        }
        else if(j->next == NULL) //голова
            del_proc(&j, 1);
        else {
            while(j->next->next != NULL)
                j = j->next;
            del_proc(&j, 2);
        }
    }
    return true;
}

static bool prev_proc(char *tmp , t_processes **processes){
    t_processes *j = *processes;

    if (mx_strcmp_null(tmp, "-") == 0) { // previous proc предпоследний
        if (j->next == NULL) {
            mx_printstr("fg: no previous proc\n");
            return false;
        }
        else if(j->next->next == NULL)
            del_proc(processes, 1);
        else {
            while(j->next->next->next != NULL)
                j = j->next;
            del_proc(&j, 2);
        }
    }
    return true;
}

static bool name_proc(char *tmp , t_processes **processes){
    t_processes *j = *processes;
    int num;

    if (mx_isalpha(tmp[0])) {
        num = mx_name_search(tmp , j);
        if(num == -1) {
            mx_printerror(1, "", tmp, "fg: proc");
            return false;
        }
        else if (num == 0)
            del_proc(&j, 1);
        else {
            while(num - 1){
                j = j->next;
                num--;
            }
            del_proc(&j, 2);
        }
    }
    return true;
}

bool proc_chars_find(char *args, t_processes **processes) {
    char *tmp = cut_str_forproc(args);
    bool flag = true;

    if (!cur_proc(tmp, processes))
        flag = false;
    if (!prev_proc(tmp, processes))
        flag = false;
    if (!name_proc(tmp, processes))
        flag = false;
    mx_strdel(&tmp);
    return flag;
}
