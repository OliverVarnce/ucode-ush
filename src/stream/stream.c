#include "ush.h"

static char *delete_button(char *line) {
    char *res = mx_strnew(mx_strlen(line) - 1);

    mx_printstr("\b \b");
    strncpy(res, line, mx_strlen(line) - 1);
    free(line);
    return res;
}

static char *delete_check(char *line, int buf, int *x) {
    int index = 0;
    char *tmp = NULL;

    if (buf == 127 && line != NULL && mx_strlen(line) > 0 && *x == 0)
        line = delete_button(line);
    else if (buf == 127 && *x > 0 && *x < mx_strlen(line)) {
        index = mx_strlen(line) - *x;
        tmp = mx_strpart(line, index);
        for (int i = *x; i > 0; i--)
            mx_printstr(" ");
        for (int i = 0; i <= *x; i++)
            line = delete_button(line);
        mx_printstr(tmp);
        line = mx_join(line, tmp);
        for (int i = 0; i < *x; i++) {
            printf("%c[1D", 27);
            fflush(stdout);
        }
    }
    return line;
}

static char *edit_line(char *line, int *x, char *ch) {
    int index = 0;
    char *tmp = NULL;

    index = mx_strlen(line) - *x;
    tmp = mx_strpart(line, index);
    for (int i = *x; i > 0; i--)
        mx_printstr(" ");
    for (int i = 0; i < *x; i++)
        line = delete_button(line);
    printf("%s%s", ch, tmp);
    line = mx_delit_fre(line, ch);
    line = mx_join(line, tmp);
    for (int i = 0; i < *x; i++) {
        printf("%c[1D", 27);
        fflush(stdout);
    }
    return line;
}

static char *delete_but(char *line, int *x) {
    int index = 0;
    char *tmp = NULL;

    index = mx_strlen(line) - *x;
    tmp = mx_strpart(line, index + 1);
    for (int i = 0; i < *x; i++)
        mx_printstr(" ");
    for (int i = 0; i < *x; i++)
        line = delete_button(line);
    printf("%s", tmp);
    line = mx_join(line, tmp);
    for (int i = 0; i < *x - 1; i++) {
        printf("%c[1D", 27);
        fflush(stdout);
    }
    (*x)--;
    return line;
}

char *mx_stream(int buf, char *line, int *x) {
    char *ch = NULL;

    ch = (char *)(&buf);
    if (buf == 127)
        line = delete_check(line, buf, x);
    if (buf == 4 && *x > 0)
        line = delete_but(line, x);
    else if (31 < buf && buf < 127) {
        if (*x != 0)
            line = edit_line(line, x, ch);
        else {
            mx_printstr(ch);
            line = mx_delit_fre(line, ch);
        }
    }
    fflush(stdout);
    return line;
}
