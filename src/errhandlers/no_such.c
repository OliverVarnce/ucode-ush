#include "ush.h"

int no_such(char *name, char *args, char *str) {
    //name`args: no such str
    //fg: %3: no such job
    write(1, name, mx_strlen(name)); //fg:
    write(1, ": ", 2);
    write(1, args, mx_strlen(args)); 
    write(1, ": no such  ", 11);
    write(1, str, mx_strlen(str));
    write(1, "\n", 1);
    return 1;
}
