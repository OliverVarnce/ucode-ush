#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <err.h>
#include <signal.h>
#include <stdarg.h>
#include <ctype.h>

#include "libmx.h"

#define MX_ISLNK(m)      (((m) & MX_IFMT) == MX_IFLNK)                  
#define MX_IFMT          0170000    
#define MX_IFLNK         0120000         /* [XSI] symbolic link */

#define CANON_OPTS 228

typedef struct s_processes {
    int num;
    char sign;
    pid_t pid;
    int index;
    char **data;
    char *pwd;
    struct s_processes *next;
}              t_processes;

typedef struct s_cd {
    int error;
    int flag_s;
    int flag_P;
    int f;
}              t_cd;

typedef struct s_history {
    void *data;
    struct s_history *next;
    struct s_history *prev;
}              t_history;

typedef struct s_env {
    int flag_i;
    int flag_u;
    int flag_P;
    char **n;
    char *clear[1];
}              t_env;

typedef struct s_ush {
    int last_return;
    int exit;
    struct s_list *pids;
    struct s_processes *processes;
    struct s_list *env_set;
    struct s_history *hist;
    struct termios savetty;
}              t_ush;

#define MX_FUNC_SYMBOLS "\"\'$`(){}\\;"
#define MX_SLASH_SPEC_DBLQ "`$\"\\"
#define MX_SLASH_SPEC "`$\"\\ '(){};~"

#define MX_ECHO_ESC_SPEC_CHAR "\a\b\x1b\f\n\r\t\v\\"
#define MX_ECHO_LITERAL_SPEC_CHAR "abefnrtv\\"

#define MX_ERR_PARSE_UNMATCH "ush: parse error: unmatched "
#define MX_ERR_PARSE_CMDSBN "ush: parse error in command substitution\n"
#define MX_ERR_PARSE_BADSBN "ush: bad substitution\n"
#define MX_ERR_PARSE_CLSBRC "ush: closing brace expected\n"
#define MX_ERR_PARSE_CLSPAR "ush: closing parenthesis expected\n"
#define MX_ERR_PARSE_CLSSINQ "ush: closing single quote expected\n"
#define MX_ERR_PARSE_CLSBCKQ "ush: closing backquote expected\n"
#define MX_ERR_PARSE_UNESCDOL "ush: unescaped dollar sign\n"
#define MX_ERR_PARSE_UNESCOPPAR "ush: unescaped (\n"
#define MX_ERR_PARSE_UNESCCLPAR "ush: unescaped )\n"
#define MX_ERR_PARSE_UNESCOPBRC "ush: unescaped {\n"
#define MX_ERR_PARSE_UNESCCLBRC "ush: unescaped }\n"
#define MX_ERR_PARSE_NO_SUCH_USER "ush: no such user or named directory: "
#define MX_IS_SP_TAB_NL(c) ((c == '\n') || (c == '\t') || (c == ' '))

enum e_quote {
    SIN_Q,
    DBL_Q,
    TDBL_Q,  // temporary opened stack flag
    BCK_Q,
    TBCK_Q,  // temporary opened stack flag
    DOL_CMD,
    TDOL_CMD,  // temporary opened stack flag
    DOL_P,
    DOL_BP,
    SLASH,
    TSLASH,  // temporary opened stack flag
    SEMICOL,
    OUT_CMDS,  // outermost command substitutions list - both ` ` and $()
    OUT_DBQ,  // outermost double quotes list (also not inside of ` ` and $()
    OUT_SUB,
    NUM_Q
};

enum e_spec_ch_mark {
    M_SKP = -10,  // skip this char
    M_SKSL,  // slash to be skipped
    M_DEL,  // whitespace word separator delimeter
    M_SEMIC,  // command separator semicolon
    NUM_MC = 4
};

enum e_echo_flags {
    ECHO_NONL,
    ECHO_SPEC,
    ECHO_NOSPEC,
    NUM_ECHO_FLAGS
};

typedef struct s_quotes_params_data {
    int start;
    int end;
}              t_range;

typedef struct s_formatting_list {
    struct s_quotes_params_data *data;
    struct s_formatting_list *next;
}              t_frmt_lst;

int mx_check_backquote(char *s, int *i, t_frmt_lst **arr);
int mx_check_single_quote(char *s, int *i, t_frmt_lst **arr);
int mx_check_dollar(char *s, int *i, t_frmt_lst **arr);
void mx_pop_format(t_frmt_lst **del);
void mx_push_format(t_frmt_lst **add, int start, int end, t_frmt_lst **del);
void mx_push_back_format(t_frmt_lst **add, int start, int end,
                         t_frmt_lst **del);
void mx_free_format_lists(t_frmt_lst **arr);
void mx_replace_sub_str(char **s, int start, int end, char *replace);
int mx_check_double_quote(char *s, int *i, t_frmt_lst **arr);
int mx_check_open_paren(char *s, int *i, t_frmt_lst **arr);
int mx_check_close_paren(char *s, int *i, t_frmt_lst **arr);
int mx_check_open_brace(char *s, int *i, t_frmt_lst **arr);
int mx_check_close_brace(char *s, int *i, t_frmt_lst **arr);
int mx_get_format_str(char *s, t_frmt_lst **arr);
char *mx_get_subst_replace_str(char **str, t_frmt_lst *list, t_ush *ush);
void mx_mark_slash_dbl_single_quote(char *s, t_frmt_lst **arr);
void mx_mark_semicolon(char *s, t_frmt_lst **arr);
void mx_mark_chars(char *s, t_frmt_lst **arr);
char *mx_clear_str(char *str);
void mx_quit_parse(char *line, t_ush *ush, int ret_val, 
                   t_frmt_lst **arr );
int mx_parse_exec(char *sub_line, t_ush *ush);
int mx_semicolon_split(char *line, t_ush *ush, char ***sub_commands);
void mx_create_outer_subst_n_double_q_list(char *s, t_frmt_lst **arr);
char *mx_get_subst_outputs(char *str, t_ush *ush);
t_range *mx_is_inside_of(int i, enum e_quote type, t_frmt_lst **arr);
int mx_handle_substitutions(char **str, t_frmt_lst **arr, t_ush *ush);
int mx_tilde_expansion(char **argv);
int mx_ush_echo(char **argv);
char mx_getopt(int argc, char **argv, char *optstring, int *optind);
void mx_enable_canon(void);
void mx_disable_canon(void);
struct termios *mx_get_tty(void);
void mx_init_signals(void);
void prepare_tty(void);
bool mx_check_parse(char *str);
void mx_setup_term(t_ush *ush);
//------------------------------------------------------------------
//------------------------------------------------------------------
void mx_push_backdup(t_list **list, void *data);
void mx_pop_frontf(t_list **head);
void mx_add_proc(t_processes **j, char **args, pid_t pid);
t_processes *mx_create_proc(char **data, int num, pid_t pid, char *pwd);
char **mx_copy_dub_arr(char **args);
void mx_free_processes(t_processes **processes);
void mx_history_replenish(t_history **hs, char *data);
int ush_cd(char **args);
int ush_env(char **args, t_processes **processes);
int ush_exit(char **args, t_ush *ush);
int ush_pwd(char **args);
bool mx_opencheck(char *dirname, t_cd *in);
char *mx_read_stream(t_history *h);
void mx_free_history(t_history **list);
void mx_free_list(t_list **list);
int ush_which(char **args);
int mx_empty_proc(char **args, t_processes **processes);
int mx_parse(char *line, t_ush *ush);
int detect_builds(char **args, t_ush *ush);
char *mx_strpart(char *str, int index);
char *mx_stream(int buf, char *line, int *x);
void free_node(t_history *node);
int detect_builds(char **args, t_ush *ush);

int mx_detect_exp(char **proc, t_history *start_h, t_list **env_set);
void mx_env_in_list(t_list **env_set, char *src);
int ush_export(char **args, t_list **env_set);
int ush_unset(char **args, t_list **env_set);
int ush_fg(char **args, t_processes **processes);
int mx_printerror(int errnum, char *name, char *args, char *str);

void mx_del_proc(t_processes **processes, int flag);
char *cut_str_forproc(char *args);
bool proc_num_find(char *args, t_processes **processes);
bool proc_chars_find(char *args, t_processes **processes);
int mx_name_search(char *tmp , t_processes *processes);
int ush_processes(char **args, t_processes **processes);
char *mx_delit_fre(char *src, char *d);
char *mx_join(char *src, char *d);
int mx_strcmp_null(const char *s1, const char *s2);
int mx_print_history(t_ush *ush);
int mx_env_print(void);
void mx_sig_listener();
void mx_sig_def();
int mx_check_max(t_processes *j);

#endif
