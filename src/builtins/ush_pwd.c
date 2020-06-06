#include "ush.h"

int ush_pwd(char **args) {
    char t[512];
    char *pwd = getcwd(t, 512);

    if (args[1] != NULL && args[1][0] == '-') {
        if (mx_get_char_index(args[1], 'P') > -1) {
            mx_printstr(pwd);
            mx_printstr("\n");
            return 2;
        }
    }
    mx_printstr(getenv("PWD"));
    mx_printstr("\n");
    return 2;
}
