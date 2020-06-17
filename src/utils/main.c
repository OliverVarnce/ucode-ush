#include "ush.h"

static void start_loop(t_ush *ush) {
    char *line = NULL;
    ush->hist = NULL;
    ush->env_set = mx_create_node(NULL);

   for (; CANON_OPTS;) {
        mx_enable_canon();
        mx_printstr("u$h> ");
        line = mx_read_stream(ush->hist);
    //char *line = strdup("env -i emacs");
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
    tcsetattr(0, TCSAFLUSH, &ush->savetty);
    mx_free_list(&ush->env_set);
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

void mx_mysetenv() {
    struct passwd *pw = getpwuid(getuid());
    char *pwd = getcwd(NULL, 0);
    if (getppid() == 1)
        return;
    setenv("HOME", pw->pw_dir, 0);
    setenv("LOGNAME", getlogin(), 0);
    setenv("PWD", pwd, 0);
    setenv("OLDPWD",pwd, 0);
    setenv("SHLVL", "1", 0);
    setenv("_", "/usr/bin/env", 0);
    free(pwd);

}

int main() {
    int ex = 0;
    t_ush *ush = (t_ush *)calloc(6, sizeof(t_ush));

    if ((*environ) == NULL || getenv("SHLVL")) {
        mx_mysetenv();
        ush->env = true;
    }
    else
        ush->env = false;

//    for (int i = 0; environ[i] != NULL; i++)
//        printf("%s\n", environ[i]);

    ush->processes = mx_create_proc(NULL, -1, -1, NULL);

//    set_slvlup();
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
