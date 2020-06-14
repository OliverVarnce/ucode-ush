#include "ush.h"

static void mx_print_path(char *args) {
    char *tmp = NULL;
    char *path = getenv("PATH");
    char **m = NULL;
    
    m = mx_strsplit(path, ':');
    tmp = mx_strjoin(m[3], "/");
    tmp = mx_delit_fre(tmp, args);
    printf("%s\n", tmp);
    mx_strdel(&tmp);
    mx_del_strarr(&m);
}

static void print_builtin_msg(char *args, int *err_code, int flag_a,
        int *f_pass) {
    char *built[] = {"cd", "pwd", "exit", "which", "env", "fg",
        "processes", "export", "unset", "false", "true", NULL};
    int built_in = 0;

    for (int i = 0; built[i]; i++) {
        if (!mx_strcmp(built[i], args)) {
            printf("%s: shell built-in command\n", args);
            if (flag_a)
                mx_print_path(args);
            built_in = 1;
            if (*f_pass == 1)
                *err_code = 0;
        }
    }
    if (!built_in) {
        printf("%s not found\n", args);
        *err_code = 1;
    }
    *f_pass = 0;
}

int mx_which(char **args) {
    int err_code = 1;
    int flag_a = 0;
    int f_pass = 1;
    int i = 1;

    if (!args[i])
        return 1;
    else if (!mx_strcmp(args[i], "-a")) {
        flag_a = 1;
        i++;
    }
    else if (!mx_strcmp(args[i], "-s"))
        i++;
    for (; args[i]; i++)
        print_builtin_msg(args[i], &err_code, flag_a, &f_pass);
    return err_code;
}
