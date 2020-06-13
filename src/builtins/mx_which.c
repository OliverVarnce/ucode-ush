#include "ush.h"

static void to_access(char *args) {
    char *tmp = NULL;
    char *path = getenv("PATH");
    char **m = NULL;

    m = mx_strsplit(path, ':');
    for (int i = 0; m[i]; i++) {
        tmp = mx_strjoin(m[i], "/");
        tmp = mx_delit_fre(tmp, args);
        if (i == 3)
            printf("%s\n", tmp);
        mx_strdel(&tmp);
    }
    mx_strdel(&tmp);
    mx_del_strarr(&m);
}

static int builtin_check(char *args) {
    char *built[] = {"cd", "pwd", "exit", "which", "env", "fg", 
        "processes", "export", "unset", "false", "true", NULL};

    for (int i = 0; built[i]; i++) {
        if (!mx_strcmp(built[i], args)) {
            printf("%s: shell built-in command\n", built[i]);
            return 0;
        }
    }
    return 1;
}

int mx_which(char **args) {
    int error_code = 0;

    if (args[1] == NULL || args[2] == NULL)
        return 1;
    for (int i = 1; args[i]; i++) {
        if (mx_strcmp(args[i], "-a") != 0 && mx_strcmp(args[i], "-s") != 0) {
            if (builtin_check(args[i])) {
                mx_printstr(args[i]);
                mx_printstr(" not found\n");
                error_code = 1;
            }
            if (error_code == 0)
                to_access(args[i]);
        }
        else if (mx_strcmp(args[i], "-a") == 0) {
            i++;
            if (builtin_check(args[i])) {
                mx_printstr(args[i]);
                mx_printstr(" not found\n");
                error_code = 1;
            }
            if (error_code == 0)
                to_access(args[i]);
        }
        else if (mx_strcmp(args[i], "-s") == 0) {
            i++;
            if (builtin_check(args[i])) {
                mx_printstr(args[i]);
                mx_printstr(" not found\n");
                error_code = 1;
            }
        }
    }
    return error_code;
}
