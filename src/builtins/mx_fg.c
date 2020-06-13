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

static t_processes* get_process(int n, char *str, t_processes *processes) {
    t_processes *tmp = processes;

    if (n != -1) {
        while (tmp) {
            if (((t_processes*)tmp->data)->index == n)
                return ((t_processes*)tmp->data);
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    else {
        while (tmp) {
            if (mx_is_str_starts(tmp->data[0], str))
                return ((t_processes*)tmp->data);
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    return 0;
}

static void fg_wait(int status, pid_t ch_pr, t_processes *processes) {
    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGSEGV)
            mx_segfault();
        else if (MX_WTERMSIG(status) == SIGINT) {
            mx_del_pid_process(&processes, ch_pr);
            processes->last_status = 130;
        }
        else {
            char **str = mx_get_name(processes, ch_pr);
            mx_print_suspended(str);
        }
    }
}

static int fg_continue(char **arg, t_processes *processes) {
    t_processes *proc = (t_processes*)processes;
    int i = 0;

    if (arg[1] == 0) {
        kill(proc->pid, SIGCONT);
        return 0;
    }
    i = (arg[1][0] == '%') ? 1 : 0;
    if (mx_is_number_fg(arg[1])) {
        proc = get_process(mx_atoi(&arg[1][i]), arg[1], processes);
    }
    else {
        proc = get_process(-1, &arg[1][i], processes);

    }
    if (proc == 0) {
        return 1;
    }
    mx_print_continue(proc->data, proc->index);
    kill(proc->pid, SIGCONT);
    return 0;
}

char **mx_get_name(t_processes *processes, int num) {
    t_processes *head_pr = processes;

    while (head_pr) {
        if (head_pr->pid == num) {
            return head_pr->data;
        }
        head_pr = head_pr->next;
    }
    return 0;
}

void mx_del_top_process(t_processes *processes) {

    if (!processes)
        return;
    mx_del_strarr(&((t_processes*)processes)->data);
    //mx_pop_front(&processes);
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

char *mx_strj(char *s1, char *s2) {
    char *s = NULL;
    int a;

    if (s1 != 0 && s2 != 0) {
        s = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
        a = mx_strlen(s1);
        if (s != 0){
            for (int i = 0 ; i < a; i++)
                s[i] = s1[i];
            for (int i = 0; i < mx_strlen(s2); a++, i++)
                s[a] = s2[i];
        }
        free(s1);
        return s;
    }
    if (s1 == 0 && s2 == 0)
        return 0;
    if (s1 != 0)
        return mx_strdup(s1);
    return mx_strdup(s2);
}

bool mx_is_str_starts(char *string, char *start) {
    if (mx_strncmp(string, start, mx_strlen(start)) == 0)
        return true;
    return false;
}

char *mx_arrstr_to_str(char **strs) {
    char *new_string = 0;
    int i = -1;

    while (strs[++i]) {
        new_string = mx_strj(new_string, strs[i]);
        if (strs[i + 1])
            new_string = mx_strj(new_string, " ");
    }
    return new_string;
}

void mx_print_continue(char **mas_name, int index) {
    char *name = mx_arrstr_to_str(mas_name);

    printf("[%d] + continued %s\n",index, name);
    free(name);
}

void mx_print_suspended(char **mas_name) {
    char *name = mx_arrstr_to_str(mas_name);

    printf("\nush: suspended %s\n", name);
    free(name);
}

void mx_segfault() {
        mx_printerr("ush: segmentation fault\n");
        mx_print_unicode(0x1f92d);
        mx_printstr("\n");
}


void mx_fg(char **args, t_processes *processes) {
    pid_t ch_pr = 0;
    int status = 0;
    printf("Args:%s\n", *args);
    printf("%s\n", *processes->data);
    printf("%d\n", processes->last_status);
    printf("%d\n", processes->pid);
    printf("%d\n", processes->index);

    if (processes != NULL) {
        if (fg_continue(args, processes) == 0) {
            ch_pr = waitpid(-1, &status, WUNTRACED);
            if (!MX_WIFEXIT(status)){
                fg_wait(status, ch_pr, processes);
            }
            else {
                mx_del_pid_process(&processes, ch_pr);
                processes->last_status = MX_EXSTATUS(status);
            }
        }
    }
    else
        mx_printerr("fg: no current job\n");
}

