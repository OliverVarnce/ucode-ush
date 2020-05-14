#include "ush.h"

void mx_arch_command(t_envp *var, char** com) {
    if (com[1]) {
        mx_printerror("arch: Can't find ");
        mx_printerror(com[1]);
        mx_printerror(" in PATH\n");
        mx_envp_replace(&var, "?=1");
    }
    else {
        printf("%s\n", "i386");
        mx_envp_replace(&var, "?=0");
    }
}
