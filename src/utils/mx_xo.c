#include "ush.h"

static int get_hex_and_oct(char *buff, char *str, int *i) {
    if (str[*i + 1] == 'x' && mx_regexp(str + *i + 2, "^[0-9a-fA-F].*$")) {
        *i += 2;
        buff[0] = str[*i];
        buff[1] = str[(*i) + 1];
        (*i)++;
        return (int)strtol(buff, NULL, 16);
    }
    else if (str[*i + 1] == '0' && mx_regexp(str + *i + 2, "^[0-7]{2,3}.*$")) {
        *i += 2;
        buff[0] = str[*i];
        buff[1] = str[(*i) + 1];
        if (str[*i + 2] >= '0' && str[*i + 2] <= '7') {
            buff[2] = str[*i + 2];
            (*i)++;
        }
        (*i)++;
        return (int)strtol(buff, NULL, 8);
    }
    return -1;
}

int mx_xo(char *str, int *i) {
    char *buff = mx_strnew(3);
    int result = 0;

    result = get_hex_and_oct(buff, str, i);
    free(buff);
    return result;
}
