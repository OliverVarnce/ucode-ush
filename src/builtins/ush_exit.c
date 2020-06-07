#include "ush.h"

int ush_exit(char **args, t_ush *ush){
    if (args[1] == NULL)
        ush->exit = ush->last_return;
    else if (args[2]) {
       mx_printerr("exit: too many arguments\n");
        return 1;
    }
    else {
        ush->exit = mx_atoi(args[1]);
        if (ush->exit >= 256) {
            while (ush->exit > 256)
                ush->exit = ush->exit - 256;
        }
    }
    return 0;
}
