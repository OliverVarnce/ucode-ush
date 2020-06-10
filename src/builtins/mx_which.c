#include "ush.h"

static int to_access(char *args, int flag) {
    char *tmp = NULL;
    int f = 1;
    char *path = getenv("PATH");
    char **m = NULL;

    m = mx_strsplit(path, ':');
    for (int i = 0; m[i]; i++) {
        tmp = mx_strjoin(m[i], "/");
        tmp = mx_delit_fre(tmp, args);
        if (to_access(tmp, F_OK) == 0) {
            flag != 3 ? printf("%s\n", tmp) : 0;
            f = 0;
            if (flag == 0)
                break ;
        }
        mx_strdel(&tmp);
    }
    mx_strdel(&tmp);
    mx_del_strarr(&m);
    return f;
}

static int builtin_check(char *args) {
    char *built[] = {"cd", "pwd", "exit", "which", "env", "fg", 
        "processes", "export", "unset", "false", "true", NULL};
    
    for (int i = 0; built[i]; i++) {
        if (!mx_strcmp(built[i], args)) {
           // printf("%s: shell built-in command\n", built[i]);
            return 0;
        }
    }
    return 2;
}

int mx_which(char **args) {
    int f = 1;
    int flag = 0;

    if (args[1] == NULL)
        return 1;
    if (args[1][0] == '-' && args[1][1] == 'a') {
        //args++;
        flag = 1;
       //for (args++; *args; args++) {
           f = 1;
            if (builtin_check(*args)) {
                f = to_access(*args, flag);
                if (f == 1) {
                    mx_printstr(*args);
                    mx_printstr(" not found\n");
                }
            }
       // }
    }
    else if (args[1][0] == '-' && args[1][1] == 's') {
        flag = 0;
        f = 0;
        if (builtin_check(*args)) {
            f = to_access(*args, flag);
            if (f == 1) {
                mx_printstr(*args);
                mx_printstr(" not found\n");
            }
        }
    }

    return f;
}
