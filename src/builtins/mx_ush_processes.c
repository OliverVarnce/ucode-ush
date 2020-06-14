#include "ush.h"

static void print_sec(t_processes *processes, char *flags) {
    t_processes *j = processes;

    mx_printstr(" suspended  ");
    for (int k = 0; j->data[k]; k++) {
        mx_printstr(j->data[k]);
        if (j->data[k + 1])
            mx_printstr(" ");
    }
    mx_printstr("\n");
    if (flags != NULL && mx_get_char_index(flags, 'd') > 0) {
        mx_printstr("(pwd : ");
        if (mx_strcmp_null(j->pwd, getenv("HOME")) == 0)
            mx_printstr("~");
        else
            mx_printstr(j->pwd);
        mx_printstr(")\n");
    }
}

static void print_proc(t_processes *processes, int num, char *flags) {
    t_processes *j = processes;
  
    while(num){
        j = j->next;
        num--;
    }
    mx_printstr("[");
    mx_printint(j->num);
    mx_printstr("]");
    mx_printstr("  ");
    if (j->next == NULL)
        mx_printstr("+");
    else if (j->next->next == NULL)
        mx_printstr("-");
    else
       mx_printstr(" ");
    if (flags != NULL && (mx_get_char_index(flags, 'l') > 0 
        || mx_get_char_index(flags, 'p') > 0)){
        mx_printstr(" ");
        mx_printint(j->pid);
    }
    print_sec(j, flags); 
}

static bool check_flag(char *args) {
        for (int i = 1; args[i]; i++) {
            if (args[i] != 'd' && 
                args[i] != 'p' && args[i] != 'l'
                    && args[i] != 's') {
                        mx_printstr("processes: bad option: -");
                        mx_printchar(args[i]);
                        mx_printstr("\n");
                        return false;
            }
        }
    return true;
}

static int ch_prococ(char **args, t_processes *processes, int i, char *flags) {
    int ind;
    t_processes *j = processes;

    if ((flags == NULL && args[1] != NULL) || (flags != NULL && args[2] != NULL)) { // если не нул
        mx_printstr("+++++\n");
        for ( ; args[i]; i++) {
            ind = mx_name_search(args[i], processes);
            if (ind == -1) {
                mx_printerror(1, "", args[i], "processes: proc");
                return 1;
            }
            print_proc(processes, ind, flags);
        }
    }
    else { //если нул
        i = 0;
        for( ; j; i++, j = j->next)
            print_proc(processes, i, flags);
    }
    return 0;
}

int mx_ush_processes(char **args, t_processes **processes) {
    int i = 1;
    char *flags = NULL;
    int res = 1;
    t_processes *j = *processes;

    if (args[1] !=  NULL && args[1][0] ==  '-') {
        if (!check_flag(args[1]))
            return 1;
        i = 2;
        flags = mx_strjoin(flags, args[1]);
    }
    if ((args[1] == NULL && j->data == NULL) || 
        (flags != NULL && args[2] == NULL && j->data == NULL)) {
            if (flags != NULL)
                mx_strdel(&flags);
            return 0;
        }
    res = ch_prococ(args, j, i, flags);
    if (flags != NULL)
        mx_strdel(&flags);
    return res;
}
