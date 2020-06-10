#include "ush.h"

static int wrong_cd(char *name, t_cd *num) {
    if (num->error == 1)
        mx_printerr("cd: no such file or directory: ");
    else if (num->error  == 2)
        mx_printerr("cd: not a directory: ");
    else if (num->error  == 3)
        mx_printerr("cd: string not in pwd: ");
    else if (num->error  == 4)
        mx_printerr("cd: permission denied: ");
    mx_printerr(name);
    mx_printerr("\n");
    free(num);
    errno = 0;
    return 1;
}

char *mx_dot_back(char *newpwd) {
    char **p = mx_strsplit(newpwd, '/');
    char *tmp = NULL;

    if (p == NULL || p[1] == NULL) {
        free(newpwd);
        return mx_strdup("/");
    }
    tmp = mx_strjoin(tmp, "/");
    for (int i = 0; p[i + 1]; i++) {
        tmp = mx_delit_fre(tmp, p[i]);
        (p[i + 2] != NULL) ? tmp = mx_delit_fre(tmp, "/") : 0;
    }
    free(newpwd);
    newpwd = tmp;
    mx_del_strarr(&p);
    return newpwd;
}

int mx_env_in_run(char *newpwd, char *pwd, char *args, t_cd *in) {
    char *gcwd = getcwd(NULL, 0);

    if (chdir(newpwd) == -1) {
        free(pwd);
        in->error = 1;
        return wrong_cd(args, in);
    }
    if (in->flag_P == 1){
        setenv("PWD", gcwd, 1);
    }
    else 
        setenv("PWD", newpwd, 1);
    setenv("OLDPWD", pwd, 1);
    free(pwd);
    free(gcwd);
    errno = 0;
    free(in);
    return 2;
}

char *mx_basic_path(char *newpwd, char *m, t_cd *in) {
    struct stat inf;

    lstat(newpwd, &inf);
    if (newpwd != NULL && newpwd[0] != '/') 
        newpwd = mx_delit_fre(newpwd, "/");
    else 
        newpwd = mx_delit_fre(newpwd, "/");
    newpwd = mx_delit_fre(newpwd, m);
    if (!mx_opencheck(newpwd, in)) {
        free(newpwd);
        return NULL;
    }
    return newpwd;
}

char *mx_run(char *newpwd, char **m, t_cd *in) {
    for (int j = 0; m[j]; j++) {
        if (!mx_strcmp(m[j], "~") && j == 0)
            continue;
        else if (!mx_strcmp(m[j], "..") && newpwd != NULL &&
            mx_strcmp(newpwd, "/") != 0) {
            newpwd = mx_dot_back(newpwd);
        }
        else if (!mx_strcmp(m[j], "."))
            continue;
        else if (mx_strcmp(m[j], "..") != 0) {
            newpwd = mx_basic_path(newpwd, m[j], in);
            if (in->error > 0) {
                mx_del_strarr(&m);
                errno = 0;
                return newpwd;
            }
        }
    }
    mx_del_strarr(&m);
    return newpwd;
}
char *mx_check(char **args, int f, char *pwd) {
    char *newpwd = NULL;

    if (args[f][0] == '~')
        newpwd = mx_strdup(getenv("HOME")); 
    else if (args[f][0] != '/')
        newpwd = mx_strdup(pwd); 
    return newpwd;
}

int mx_env_return(char **args, char *pwd, t_cd *in) {
    if (args[1] == NULL || !mx_strcmp(args[in->f], "~"))
        return mx_env_in_run(getenv("HOME"), pwd, args[0], in);
    if (!mx_strcmp(args[in->f], "-") || !mx_strcmp(args[in->f], "~-"))
        return mx_env_in_run(getenv("OLDPWD"), pwd, args[in->f], in);
    if (!mx_strcmp(args[in->f], "~+"))
        return mx_env_in_run(getenv("PWD"), pwd, args[in->f], in);
    if (mx_strcmp(args[in->f], "/") == 0)
        return mx_env_in_run("/", pwd, args[in->f], in);
    return 0;
}

int mx_cd(char **args) {
    char *pwd = mx_strdup(getenv("PWD"));
    char **m = NULL;
    char *newpwd = NULL;
    t_cd *in = (t_cd *)malloc(sizeof(t_cd) * 4);
    
    in->f = 1;
    if (mx_env_return(args, pwd, in) > 0)
        return 2;
    (args[1][0] == '-') ? in->f = 2 : 0;
    in->f == 2 && mx_get_char_index(args[1], 's') > -1 ? in->flag_s = 2 : 0;
    in->f == 2 && mx_get_char_index(args[1], 'P') > -1 ? in->flag_P = 1 : 0;
    newpwd = mx_check(args, in->f, pwd);
    m = mx_strsplit(args[in->f], '/');
    newpwd = mx_run(newpwd, m, in);
    if (newpwd == NULL) {
        free(pwd);
        return wrong_cd(args[in->f], in);
    }
    mx_env_in_run(newpwd, pwd, args[in->f], in);
    free(newpwd);
    return 2;
}
