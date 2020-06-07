#include "ush.h"

static void start_loop(t_ush *ush) {
    char *line = NULL;
    ush->hist = NULL;
    ush->env_set = mx_create_node(NULL);

    mx_sig_listener();
    for (; CANON_OPTS;) {
        mx_enable_canon();
        mx_printstr("u$h> ");
        line = mx_read_stream(ush->hist);
        mx_printstr("\n");
        if (line != NULL && mx_strlen(line) > 0) {
            mx_history_replenish(&ush->hist, line);
            mx_disable_canon();
            mx_parse(line, ush);
            system("leaks -q ush");
        }
        if (ush->exit >= 0)
            break;
    }
    mx_sig_def();
    tcsetattr(0, TCSAFLUSH, &ush->savetty);
    mx_free_list(&ush->env_set);
}

static void pipe_call(t_ush *ush) {
    int buf = 0;
    char *line = NULL;
    char *ch = NULL;

    ush->env_set = mx_create_node(NULL);
    while (read(STDIN_FILENO, &buf, 3) > 0) {
        ch = (char *)(&buf);
        line = mx_delit_fre(line, ch);
        buf = 0;
    }
    mx_parse(line, ush);
    mx_free_list(&ush->env_set);
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
        mx_free_history(&ush->hist);
        exit(ex);
    }
    else
        pipe_call(ush);
    mx_free_processes(&ush->processes);
    free(ush);
    return 0;
}
