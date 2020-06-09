#include "ush.h"

void mx_replace_sub_str(char **s, int start, int end, char *replace) {
    if (end >= start) {
        char *new;

        if (!s || !*s)
            return;

        if (!replace)
            replace = "";
        new = calloc(1, mx_strlen(*s) - (end - start) + mx_strlen(replace));
        mx_strncpy(new, *s, start);
        mx_strcpy(new + start, replace);
        mx_strcat(new + start, *s + end + 1);
        mx_strdel(s);
        *s = new;
    }
    else
        return ;
}
