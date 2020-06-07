#include "ush.h"

int mx_empty_proc(char **args, t_processes **processes) {
    pid_t pid;
    int status = 0;
    
    pid = fork();
    if (pid == 0) {
        execvp(*args, args);
        mx_printerror(1, "", args[0], "ush: command");
        exit(127);
    }
    for (waitpid(pid, &status, WUNTRACED);
        !WIFEXITED(status) && !WIFSIGNALED(status);
        waitpid(pid, &status, WUNTRACED)) {
        if (WIFSTOPPED(status)) {
            mx_add_proc(processes, args, pid);
            break ;
        }
    }
    errno = 0;
    return WEXITSTATUS(status);
}
