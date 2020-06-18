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

void mx_printpros(t_processes *tmp) {
    if (tmp) {
        while (tmp) {
            printf("********************************************************\n");
            if (tmp->data) {
                printf("DATA: ");
                for (int i = 0; tmp->data[i]; i++)
                    printf("%s ", tmp->data[i]);
                printf("\n");
            }
            if (tmp->pwd) {
                printf("PWD: ");
                printf("%s\n", tmp->pwd);
            }
            if (tmp->index) {
                printf("INDEX: ");
                printf("%d\n", tmp->index);
            }
            if (tmp->pid) {
                printf("PID: ");
                printf("%d\n", tmp->pid);
            }

            tmp = tmp->next;
            printf("********************************************************\n");
        }
    }
}

static t_processes* get_process(int n, char *str, t_ush *ush) {
    t_processes *tmp = ush->processes;
    mx_printpros(tmp);
    if (n != -1) {
        while (tmp->next != NULL) {
            if (tmp->index == n)
                return tmp;
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    else {
        while (tmp) {
            if (mx_is_str_starts(tmp->data[0], str))
                return tmp;
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    return 0;
}

static void fg_wait(int status, pid_t ch_proc, t_ush *ush) {
    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGSEGV) {
            mx_segfault();
        }
        else if (MX_WTERMSIG(status) == SIGINT) {
            mx_del_pid_process(ush, ch_proc);
            ush->last_return = 130;
        }
        else {
            char **str = mx_get_name(ush, ch_proc);
            mx_print_suspended(str);
        }
    }
}

static int fg_continue(char **arg, t_ush *ush) {
    t_processes *proc = ush->processes;
    int i = 0;

    if (arg[1] == 0) {
        kill(proc->pid, SIGCONT);
        return 0;
    }
    i = (arg[1][0] == '%') ? 1 : 0;
    if (mx_is_number_fg(arg[1])) {
        proc = get_process(mx_atoi(&arg[1][i]), arg[1], ush);
    }
    else {
        proc = get_process(-1, &arg[1][i], ush);

    }
    if (proc == 0) {
        return 1;
    }
    mx_print_continue(proc->data, proc->index);
    kill(proc->pid, SIGCONT);
    return 0;
}

char **mx_get_name(t_ush *ush, int num) {
    t_processes *head_pr = ush->processes;

    while (head_pr) {
        if (head_pr->pid == num) {
            return head_pr->data;
        }
        head_pr = head_pr->next;
    }
    return 0;
}

void mx_del_top_process(t_ush *ush) {
    if (!ush->processes) {
        return;
    }
    if (ush->processes->data != NULL)
        mx_del_strarr(&(ush->processes->data));
    mx_strdel(&(ush->processes->pwd));
    mx_pop_front_proc(&ush->processes);
}

void mx_del_pid_process(t_ush *ush, int pid) {
    t_processes *tmp = ush->processes;
    t_processes *tmp2 = ush->processes;

    if (ush->processes) {
        if (tmp->pid == pid) {
            mx_del_top_process(ush);
            return;
        }
        while (tmp->next) {
            if (tmp->pid == pid) {
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


int mx_fg(char **args, t_ush *ush) {
    pid_t ch_proc = 0;
    int status = 0;
    t_processes *procs = ush->processes;

    if (procs->data != NULL) {
        mx_printpros(procs);
        if (fg_continue(args, ush) == 0) {
            ch_proc = waitpid(-1, &status, WUNTRACED);
            if (MX_WIFEXIT(status)){
                fg_wait(status, ch_proc, ush);
            }
            else {
                mx_del_pid_process(ush, ch_proc);
                ush->last_return = MX_EXSTATUS(status);
            }
        }
    }
    else {
        mx_printerr("fg: no current job\n");
        return 1;
    }
    return 0;
}

