#include "ush.h"

static void start_loop(t_ush *ush) {
    char *line = NULL;
    ush->hist = NULL;
    ush->env_set = mx_create_node(NULL);

    for (; CANON_OPTS;) {
        mx_enable_canon();
        write(1, "u$h> ", mx_strlen("u$h> "));
        line = mx_read_stream(ush->hist);
        write (1, "\n", 1);
        if (line != NULL && mx_strlen(line) > 0) {
            mx_history_replenish(&ush->hist, line);
            mx_disable_canon();
            mx_parse(line, ush);
            system("leaks -q ush");
        }
        if (ush->exit >= 0)
            break;
    }
    tcsetattr(0, TCSAFLUSH, &ush->savetty);
    free_list2(&ush->env_set);
}

static void pipe_call(t_ush *ush) {
    int buf = 0;
    char *line = NULL;
    char *ch = NULL;

    while (read(STDIN_FILENO, &buf, 3) > 0) {
        ch = (char *)(&buf);
        line = mx_delit_fre(line, ch);
        buf = 0;
    }
    mx_parse(line, ush);
}

static void set_slvlup() {
    char *s;

    if (getppid() == 1)
        return;
    if (getenv("SHLVL")) {
        s = mx_itoa((mx_atoi(getenv("SHLVL"))) + 1);
        setenv("SHLVL", s, 1);
        free(s);
    }

}

int main() {
    int ex = 0;
    t_ush *ush = (t_ush *)calloc(6, sizeof(t_ush));
    ush->processes = mx_create_proc(NULL, -1, -1, NULL);

    set_slvlup();
    ush->exit = -1;
    if (isatty(0)) {
        mx_setup_term(ush);
        start_loop(ush);
        ex = ush->exit;
        free_list(&ush->hist);
        exit(ex);
    }
    else
        pipe_call(ush);
    free_processes(&ush->processes);
    free(ush);
    return 0;
}
