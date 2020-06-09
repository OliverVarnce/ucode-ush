#include "ush.h"

static char **env_cpy(char **environ) {
    char **res = NULL;
    int len = 0;

    while(environ[len])
        len++;
    res = (char **)malloc(sizeof(char *) * (len + 1));
    res[len] = NULL;
    for (int i = 0; environ[i]; i++) 
        res[i] = mx_strdup(environ[i]);
    return res;
}

static int usage(int f, char *arg) {
    if (f == 1)
        write(2, "usage: env [-iu] [name=value ...] "
                "[utility [argument ...]]\n", 58);
    else {
        mx_printerr("env: '");
        mx_printerr(arg);
        mx_printerr("': No such file or directory\n");
        exit(0);
    }
	return 1;
}

static void env_run(char ***environ, t_env *env) {
    char **m = NULL;

    if (env->flag_i != 1) {
        *environ = env->clear;
        env->clear[0] = NULL;
    }
    for (int i = 0; env->clear[i]; i++) {
        env->clear[i] = NULL;
    }
    for (int i = 0; env->n[i]; i++) {
        m = mx_strsplit(env->n[i], '=');
//        for (int j = 0; m[j]; j++) {
//            printf("%s\n", m[j]);
//        }
//        printf("OLEG\n");
        if (m[0] && m[1])
            setenv(m[0], m[1], 0);
        mx_del_strarr(&m);
    }
    mx_del_strarr(&env->n);
    free(env);
}

static int check_arg(char ***args, t_env *env, char ***environ) {
    if ((*args)++ && mx_get_char_index(**args, '-') == 0) {
        if (mx_get_char_index(**args, 'i') > 0) {
            env->flag_i = 1;
            *environ = env->clear;
            env->clear[0] = NULL;
            (*args)++;
        }
        else if (mx_get_char_index(**args, 'u')) {
            env->flag_u = 1;
            (*args)++;
        }
        else if (mx_get_char_index(**args, '-') != 1)
            return usage(1, NULL);
    }
    return 0;
}

int mx_env(char **args, t_processes **processes) {
    t_env *env = (t_env *)malloc(7 * sizeof(t_env));
    int status = 0;

    env->n = env_cpy(environ);
    if (check_arg(&args, env, &environ) > 0)
        return 1;
    if (env->flag_i != 1 && env->flag_u == 1) {
        unsetenv(*args);
        args++;
    }
    else
        for ( ; *args && mx_get_char_index(*args, '=') > -1; args++)
            putenv(*args);
    if (*args)
        status = mx_empty_proc(args, processes);
    else for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    env_run(&environ, env);
    if (status == -1)
        return usage(2, *args);
    return 0;
}
