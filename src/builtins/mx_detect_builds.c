#include "ush.h"

int mx_builtin(char **args, t_ush *ush) {
    if (!mx_strcmp(args[0], "echo"))
        return mx_echo(args);
    if (!mx_strcmp(args[0], "cd"))
        return mx_cd(args);
    if (!mx_strcmp(args[0], "pwd"))
        return mx_pwd(args);
    if (!mx_strcmp(args[0], "env"))
        return mx_env(args, &ush->processes);
    if (!mx_strcmp(args[0], "exit"))
        return mx_exit(args, ush);
    if (!mx_strcmp(args[0], "which"))
        return mx_which(args);
    if (!mx_strcmp(args[0], "export"))
        return mx_export(args, &ush->env_set);
    if (!mx_strcmp(args[0], "unset"))
        return mx_unset(args, &ush->env_set);
    if (!mx_strcmp(args[0], "fg"))
        return mx_fg(args, &ush->processes);
    if (!mx_strcmp(args[0], "processes"))
        return mx_ush_processes(args, &ush->processes);
//    if (!mx_strcmp(args[0], "false"))
//        return 1;
//    if (!mx_strcmp(args[0], "true"))
//        return 0;
    if (!mx_strcmp(args[0], "history"))
        return mx_print_history(ush);
    return -1;
}

int mx_detect_builds(char **args, t_ush *ush) {
    int bins;

    if (!args || !args[0])
        return 0;
    if ((bins = mx_builtin(args, ush)) != -1)
        return bins;
    bins = mx_detect_exp(args, ush->hist, &ush->env_set);;
    if (bins != 3) 
        return bins;
    else
        return mx_empty_proc(args, &ush->processes);
}
