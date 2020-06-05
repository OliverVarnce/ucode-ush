#include "ush.h"

void mx_quit_parse(char *line, t_ush *ush, int ret_val,
                       t_frmt_lst **arr ) {
    mx_free_format_lists(arr);
    if (line)
        free(line);
    if (ret_val >= 0)
        ush->last_return = ret_val;
}

int mx_parse_exec(char *sub_line, t_ush *ush) {
    t_frmt_lst *arr[NUM_Q] = {NULL};
    char **argv;

    mx_get_format_str(sub_line, arr);
    mx_mark_slash_dbl_single_quote(sub_line, arr);
    mx_mark_chars(sub_line, arr);

    if (mx_handle_substitutions(&sub_line, arr, ush) == -1) {
        mx_quit_parse(NULL, ush, 1, arr);
        return -1;
    }
    sub_line = mx_clear_str(sub_line);
    argv = mx_strsplit(sub_line, M_DEL);
    mx_quit_parse(sub_line, ush, mx_tilde_expansion(argv) == -1 ?
                                 1 : detect_builds(argv, ush), arr);
    mx_del_strarr(&argv);
    return 0;
}

int mx_semicolon_split(char *line, t_ush *ush, char ***sub_commands) {
    t_frmt_lst *arr[NUM_Q] = {NULL};

    if (mx_get_format_str(line, arr) < 0) {
        mx_quit_parse(line, ush, 1, arr);
        return -1;
    }
    mx_mark_semicolon(line, arr);
    mx_free_format_lists(arr);
    *sub_commands = mx_strsplit(line, M_SEMIC);
    return 0;
}

int mx_parse(char *line, t_ush *ush) {
    char **sub_commands = {NULL};

    if (mx_semicolon_split(line, ush, &sub_commands) == -1) {
        return -1;
    }
    for (char **s = sub_commands; *s; s++)
        mx_parse_exec(strdup(*s), ush);
    mx_del_strarr(&sub_commands);
    mx_quit_parse(line, ush, -1, NULL);
    return 0;
}
