#include "ush.h"

int builtin(char **args, t_ush *ush) {
    if (!mx_strcmp(args[0], "echo"))
        return mx_ush_echo(args);
    if (!mx_strcmp(args[0], "cd"))
        return ush_cd(args);
    if (!mx_strcmp(args[0], "pwd"))
        return ush_pwd(args);
    if (!mx_strcmp(args[0], "env"))
        return ush_env(args, &ush->processes);
    if (!mx_strcmp(args[0], "exit"))
        return ush_exit(args, ush);
    if (!mx_strcmp(args[0], "which"))
        return ush_which(args);
    if (!mx_strcmp(args[0], "export"))
        return ush_export(args, &ush->env_set);
    if (!mx_strcmp(args[0], "unset"))
        return ush_unset(args, &ush->env_set);
    if (!mx_strcmp(args[0], "fg"))
        return ush_fg(args, &ush->processes);
    if (!mx_strcmp(args[0], "processes"))
        return ush_processes(args, &ush->processes);
    if (!mx_strcmp(args[0], "false"))
        return 1;
    if (!mx_strcmp(args[0], "true"))
        return 0;
    if (!mx_strcmp(args[0], "history"))
        return mx_print_history(ush);
    return -1;
}

int detect_builds(char **args, t_ush *ush) {
    int bins;

    if (!args || !args[0])
        return 0;
    if ((bins = builtin(args, ush)) != -1) 
        return bins;
    bins = mx_detect_exp(args, ush->hist, &ush->env_set);;
    if (bins != 3) 
        return bins;
    else
        return mx_empty_proc(args, &ush->processes);
}
