#include "ush.h"

int mx_printerror(int errnum, char *name, char *args, char *str) {
    if (errnum == 1) {
        mx_printerr(str);
        mx_printerr(" not found: ");
        mx_printerr( args);
        mx_printerr("\n");
    }
    else if (errnum == 0) {
        mx_printerr(name);
        mx_printerr(": ");
        mx_printerr(args);
        mx_printerr(": no such  ");
        mx_printerr(str);
        mx_printerr("\n");
    }
    return 1;
}
