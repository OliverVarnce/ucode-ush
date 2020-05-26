#include "ush.h"

static bool all_digits(char *args){
    for (int i = 1; args[i]; i++) {
        if (!mx_isdigit(args[i]))
            return false;
    }
    return true;
}

static bool pres_persent(char *args, t_jobs **jobs) {
    if (mx_isalpha(args[1]) || args[1] == '-' 
        || args[1] == '+' || args[1] == '%'|| args[1] == '\0') {
        if (!job_chars_find(args, jobs))
            return false;
    }
    else if (all_digits(args)) {
        if (!job_num_find(args, jobs))
            return false;
    }
    else {
        mx_printerror(1, "", args, "fg: job");
        return false;
    }
    return true; 
}

int ush_fg(char **args, t_jobs **jobs) {
    if (args[1] == NULL) {
        if (job_chars_find("%%", jobs))
            return 1;
        else 
           return 0; 
    }
    for(int i = 1; args[i]; i++) {
        if (mx_get_char_index(args[i], '%') == 0) {
            if (!pres_persent(args[i], jobs))
                return 1;
        }
        else if (mx_isalpha(args[i][0])) {
            if (!job_chars_find(args[i], jobs))
                return 1;
        }
        else 
            return mx_printerror(1, "", args[i], "fg: job");
    }
    return 0;
}

