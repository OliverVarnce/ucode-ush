#include "ush.h"

int ush_pwd(char **args) {
    char t[512];
    char *pwd = getcwd(t, 512);

    if (args[1] != NULL && args[1][0] == '-') {
        if (mx_get_char_index(args[1], 'P') > -1) {
            write(1, pwd, strlen(pwd));
            write(1, "\n", 1);
            return 2;
        }
    }
    write(1, getenv("PWD"), strlen(getenv("PWD")));
    write(1, "\n", 1);
    return 2;
}
