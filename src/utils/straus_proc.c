#include "ush.h"

int straus_proc(char **args, t_jobs **jobs) {
    pid_t pid;
    int status = 0;
    
    pid = fork();
    if (pid == 0) {
        execvp(*args, args);
        not_found(args[0], "ush: command");
        exit(127);
    }
    for (waitpid(pid, &status, WUNTRACED);
        !WIFEXITED(status) && !WIFSIGNALED(status);
        waitpid(pid, &status, WUNTRACED)) {
        if (WIFSTOPPED(status)) {
            add_job(jobs, args, pid);
            break ;
        }
    }
    errno = 0;
    return WEXITSTATUS(status);
}
