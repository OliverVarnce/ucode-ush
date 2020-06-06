#include "ush.h"

static char *history_button(char *line, char *data, int *x) {
    for (; *x > 0; (*x)--) {
        printf("%c[1C", 27);
        fflush(stdout);
    }

    for (int i = 0; line != NULL && i < mx_strlen(line); i++)
        mx_printstr("\b \b");

    mx_strdel(&line);
    mx_printstr(data);
    line = mx_strdup(data);
    return line;
}

static char *button(t_history **hs, char *line, int buf, int *x) {
    t_history *h = *hs;

    if (buf == 4283163) {
        h->next ? h = h->next : 0;
        line = history_button(line, h->data, x);
    }
    if (buf == 4348699) {
        h->prev ? h = h->prev : 0;
        line = history_button(line, h->data, x);
    }
    if (buf == 4414235 && *x > 0) {
        printf("%c[1C", 27);
        (*x)--;
    }
    if (buf == 4479771 && line != NULL && mx_strlen(line) > *x) { 
        printf("%c[1D", 27);
        (*x)++;
    }
    *hs = h;
    fflush(stdout);
    return line;
}

char *mx_read_stream(t_history *h) {
    unsigned int buf = 0;
    char *line = NULL;
    int len = 0;
    int x = 0;
    t_history *head = NULL;

    mx_history_replenish(&h, "\0");
    head = h; 
    for (;(len = read(0, &buf, 4)) > 0;) {
        if (len == 1) {
            if (buf == 10 || buf == 12 || (buf == 4 && 
                (line == NULL || !mx_strlen(line)))) {
                buf == 4 ? line = mx_strdup("exit") : 0;
                buf == 12 ? line = mx_strdup("clear") : 0;
                break ;
            }
            line = mx_stream(buf, line, &x);
        }
        if (len > 1) 
            line = button(&h, line, buf, &x);
        buf = 0;
    }
    free_node(head);
    return line;
}
