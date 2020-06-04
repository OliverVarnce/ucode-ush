#include "ush.h"

char **mx_cont_var(char *var, char *tmp) {
    char **a;
    char **b;
    char **res = NULL;
    char **exp = mx_strsplit(tmp, ' ');
  
    a = mx_strsplit(var, '=');
    for (int i = 1; exp[i]; i++) {
        b = mx_strsplit(exp[i], '=');
        if (mx_strcmp_null(b[0], a[0]) == 0) {
            res = (char **) malloc(sizeof(char *) * 3);
            res[0] = mx_strdup("export");
            res[1] = mx_strdup(a[0]);
            res[2] = NULL;
            mx_del_strarr(&a);
            mx_del_strarr(&b);
            mx_del_strarr(&exp); 
            return res;
        }
        mx_del_strarr(&b);
    }
    mx_del_strarr(&a);
    mx_del_strarr(&exp); 
    return res;  
}

int mx_find_exp_h(char *str, char *var, t_list **env_set) {
    char **tmp = NULL;
    int res = 0;
    int flag = 0;
    char **exstr = NULL;

    tmp = mx_strsplit(str, ';');
    for (int i = 0; tmp[i]; i++) {
        if (mx_get_substr_index(tmp[i], "export") >= 0) {
            exstr = mx_cont_var(var, tmp[i]);
            if (exstr != NULL) {
                flag = 1;
                if (ush_export(exstr, env_set) == 1)
                    res = 1;
                mx_del_strarr(&exstr);
            } 
        }
    }
    mx_del_strarr(&tmp);
    if (flag == 0)
        return 3;
    return res;
}

int mx_detect_exp(char **proc, t_history *start_h, t_list **env_set) {
    int tmp;

    if (mx_count_substr(proc[0], "=") == 1 && 
        mx_isalpha(proc[0][mx_get_char_index(proc[0], '=') - 1])) {
        env_in_list(env_set, proc[0]);
        for (t_history *h = start_h; h; h = h->next) {
            if (mx_get_substr_index(h->data, "export") >= 0) {
                tmp = mx_find_exp_h(h->data, proc[0], env_set);
                if (tmp != 3)
                    return tmp;
                else
                    continue;
            }
        }
        return 1;
    }
    return 3;
}
