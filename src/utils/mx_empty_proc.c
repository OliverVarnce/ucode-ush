#include "ush.h"

static void signal_worker() {
    signal(SIGTSTP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}

static int wif_check(char **args, t_processes **processes, int status,
                     pid_t pid) {
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        status = 146;
        mx_add_proc(processes, args, pid);
    }
    else if (WIFSIGNALED(status)) {
        status = 130;
        mx_printstr("\n");
    }
    else if (WIFEXITED(status))
        status =  WEXITSTATUS(status);
    errno = 0;
    return status;
}


int mx_empty_proc(char **args, t_processes **processes) {
    pid_t pid;
    int status = 0;
    
    pid = fork();
    if (pid == 0) {
        setpgid(pid, pid);
        signal_worker();
        if (isatty(0)) {
            tcsetpgrp(0, getpid());
            tcsetpgrp(1, getpid());
        }
        execvp(*args, args);
        mx_printerror(1, "", args[0], "ush: command");
        exit(127);
    }
    else
        waitpid(pid, &status, WUNTRACED);
    return wif_check(args, processes, status, pid);
}
