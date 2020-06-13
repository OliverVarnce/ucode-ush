#include "ush.h"

static bool mx_is_number_fg(char *str) {
    int i = -1;
    if (str[0] == '%')
        i++;
    while (str[++i]) {
        if (str[i] < 48 || str[i] > 57)
            return false;
    }
    return true;
}

static void fg_wait(int status, pid_t ch_pr, t_processes **processes) {
    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGSEGV)
           // mx_segfault();
            printf("sigfault============\n");
        else if (MX_WTERMSIG(status) == SIGINT) {
            mx_del_pid_process(processes, ch_pr);
            //processes->last_return = 130;
            printf("sigint=============");
        }
        else {
            //char **str = mx_get_name(info, ch_pr);
            //mx_print_susp(str);
            printf("get_name ============");
        }
    }
}

static int fg_continue(char **argv, t_processes *processes) {
    t_processes *pr = (t_processes*)processes->data;
    int i = 0;

    if (argv[1] == 0) {
        kill(pr->pid, SIGCONT);
        return 0;
    }
    i = (argv[1][0] == '%') ? 1 : 0;
    if (mx_is_number_fg(argv[1])) {
        //pr = get_process(atoi(&argv[1][i]), argv[1], info);
        printf("ddd");
    }
    else {
//        pr = get_process(-1, &argv[1][i], info);
        printf("ddwwwwwd");
    }
    if (pr == 0) {
//        info->exit_status = 1;
        printf("ddd");
        return 1;
    }
    //mx_print_cont(pr->name, pr->index);
    kill(pr->pid, SIGCONT);
    return 0;
}
void mx_del_top_process(t_processes *processes) {
    if (!processes)
        return;
    mx_del_strarr(&((t_processes*)processes)->data);
    //mx_pop_front((t_processes*)processes);
}

void mx_del_pid_process(t_processes **processes, int pid) {
    t_processes *tmp = (t_processes*)processes;
    t_processes *tmp2 = (t_processes*)processes;

    if (*processes) {
        if (((t_processes*)tmp->data)->pid == pid) {
            mx_del_top_process(*processes);
            return;
        }
        while (tmp->next) {
            if (((t_processes*)tmp->next->data)->pid == pid) {
                tmp2 = tmp->next;
                tmp->next = tmp->next->next;
                mx_del_strarr(&tmp2->data);
                free(tmp2->data);
                free(tmp2);
                return;
            }
            tmp = tmp->next;
        }
    }
}

void mx_fg(char **args, t_processes **processes) {
    pid_t ch_pr = 0;
    int status = 0;

    if (processes) {
        if (fg_continue(args, *processes) == 0) {
            ch_pr = waitpid(-1, &status, WUNTRACED);
            if (!MX_WIFEXIT(status)){
                fg_wait(status, ch_pr, processes);
                printf("sss");
            }

            else {
//                mx_del_pid_process(info, ch_pr);
//                info->last_status = MX_EXSTATUS(status);
                printf("ddd");
            }
        }
    }
    else
        mx_printerr("fg: no current job\n");
}

