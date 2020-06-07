#include "ush.h"

static int mx_hlist_size(t_history *list) {
    int size = 0;

    for (t_history *walker = list; walker != NULL; walker = walker->next, size++);

    return size;
}

static char **create_commands(t_history **list_commands) {
    int sum_command = mx_hlist_size(*list_commands) + 2;
    char **commands = (char **) malloc(sum_command * sizeof(char *));
    t_history *list = *list_commands;

    commands[0] = NULL;
    for (int i = 0; list; i++) {
        commands[i] = mx_strdup(list->data);
        list = list->next;
        commands[i + 1] = NULL;
    }
    return commands;
}

int mx_print_history(t_ush *ush) {
    char **commands = create_commands(&ush->hist);
    int sum_command = mx_hlist_size(ush->hist);

    for (int i = sum_command; commands && i > 0 && commands[i - 1]; i--) {
        if (i < 10)
            mx_printstr("  ");
        else if (i < 100)
            mx_printstr(" ");
        mx_printint(sum_command + 1 - i);
        mx_printstr("  ");
        mx_printstr(commands[i - 1]);
        mx_printstr("\n");
    }
    mx_del_strarr(&commands);
    return 0;
}
