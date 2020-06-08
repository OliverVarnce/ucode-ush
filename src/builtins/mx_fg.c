#include "ush.h"

static bool arg_is_digit(char *args){
    for (int i = 1; args[i]; i++) {
        if (!mx_isdigit(args[i]))
            return false;
    }
    return true;
}

static bool push_pers(char *args, t_processes **processes) {
    if (mx_isalpha(args[1]) || args[1] == '-' 
        || args[1] == '+' || args[1] == '%'|| args[1] == '\0') {
        if (!mx_proc_chars_find(args, processes))
            return false;
    }
    else if (arg_is_digit(args)) {
        if (!mx_proc_num_find(args, processes))
            return false;
    }
    else {
        mx_printerror(1, "", args, "fg: job");
        return false;
    }
    return true; 
}

int mx_fg(char **args, t_processes **processes) {
    if (args[1] == NULL) {
        if (mx_proc_chars_find("%%", processes))
            return 1;
        else 
           return 0; 
    }
    for(int i = 1; args[i]; i++) {
        if (mx_get_char_index(args[i], '%') == 0) {
            if (!push_pers(args[i], processes))
                return 1;
        }
        else if (mx_isalpha(args[i][0])) {
            if (!mx_proc_chars_find(args[i], processes))
                return 1;
        }
        else 
            return mx_printerror(1, "", args[i], "fg: job");
    }
    return 0;
}

