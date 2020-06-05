#include "ush.h"

void mx_replace_sub_str(char **s, int start, int end, char *replace) {
    char *new;

    if (!s || !*s)
        return;

    if (!replace)
        replace = "";
    new = calloc(1, mx_strlen(*s) - (end - start) + mx_strlen(replace));
    strncpy(new, *s, start);
    strcpy(new + start, replace);
    strcat(new + start, *s + end + 1);
    free(*s);
    *s = new;
}
