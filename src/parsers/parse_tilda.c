#include "ush.h"

static void tilde2(char **str, char ***arr) {
    if (strcmp((*arr)[0], "~+") == 0) {
        mx_replace_sub_str(str, 0, 1, getenv("PWD"));
    }
    else if (strcmp((*arr)[0], "~-") == 0) {
        mx_replace_sub_str(str, 0, 1, getenv("OLDPWD"));
    }
    else
        mx_replace_sub_str(str, 0, 0, getenv("HOME"));
    mx_del_strarr(arr);
}

static void no_such_user(char *tmp, char ***m) {
    fprintf(stderr, MX_ERR_PARSE_NO_SUCH_USER "%s\n", tmp);
    mx_del_strarr(m);
    free(tmp);
}

static int tilde(char **str) {
    char *res = NULL;
    char *tmp = NULL;
    char **m = mx_strsplit(*str, '/');

    if (!strcmp(m[0], "~+") || !strcmp(m[0], "~-") || !strcmp(m[0], "~")) {
        tilde2(str, &m);
        return 0;
    }
    if (!getpwnam((tmp = strndup(*str + 1, mx_strlen(m[0]) - 1)))) {
        no_such_user(tmp, &m);
        return -1;
    }
    mx_replace_sub_str(str, 0, mx_strlen(tmp),
        (res = mx_strjoin("/Users/", tmp)));
    mx_del_strarr(&m);
    free(res);
    free(tmp);
    return 0;
}

int mx_tilde_expansion(char **argv) {
    for (char **s = argv + 1; *s; s++) {
        if (**s != '~')
            continue;
        if (tilde(s) == -1) {
            return -1;
        }
    }
    return 0;
}
