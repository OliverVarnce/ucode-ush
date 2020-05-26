#include "ush.h"

int builtin(char **args, t_ush *ush) {
    if (!strcmp(args[0], "echo"))
        return mx_ush_echo(args);
    if (!strcmp(args[0], "cd"))
        return ush_cd(args);
    if (!strcmp(args[0], "pwd"))
        return ush_pwd(args);
    if (!strcmp(args[0], "env"))
        return ush_env(args, &ush->jobs);
    if (!strcmp(args[0], "exit"))
        return ush_exit(args, ush);
    if (!strcmp(args[0], "which"))
        return ush_which(args);
    if (!strcmp(args[0], "export"))
        return ush_export(args, &ush->env_set);
    if (!strcmp(args[0], "unset"))
        return ush_unset(args, &ush->env_set);
    if (!strcmp(args[0], "fg"))
        return ush_fg(args, &ush->jobs);
    if (!strcmp(args[0], "jobs"))
        return ush_jobs(args, &ush->jobs);
    if (!strcmp(args[0], "false"))
        return 1;
    if (!strcmp(args[0], "true"))
        return 0;
    return -1;
}

int detect_builds(char **args, t_ush *ush) {
    int bins = 1;

    if (!args || !args[0])
        return 0;
    if ((bins = builtin(args, ush)) != -1) 
        return bins;
    bins = detect_exp(args, ush->hist, &ush->env_set);;
    if (bins != 3) 
        return bins;
    else
        return straus_proc(args, &ush->jobs);
}
