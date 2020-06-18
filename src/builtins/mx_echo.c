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

bool mx_regexp(char *str, char *regular) {
    regex_t regex;
    int result;

    regcomp(&regex, regular, REG_EXTENDED);
    result = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return result == 0;
}

int mx_0_and_x(char *str, int *i) {
    char *buff = mx_strnew(3);
    int result = 0;

    result = get_hex_and_oct(buff, str, i);
    free(buff);
    return result;
}

static int print_echo_d(char *str, int *i) {
    if (str[0] == '\\' && str[1] == 'a' && ++(*i) > 0)
        return '\a';
    else if (str[0] == '\\' && str[1] == 'b' && ++(*i) > 0)
        return '\b';
    else if (str[0] == '\\' && str[1] == 't' && ++(*i) > 0)
        return '\t';
    else if (str[0] == '\\' && str[1] == 'n' && ++(*i) > 0)
        return '\n';
    else if (str[0] == '\\' && str[1] == 'v' && ++(*i) > 0)
        return '\v';
    else if (str[0] == '\\' && str[1] == 'f' && ++(*i) > 0)
        return '\f';
    else if (str[0] == '\\' && str[1] == 'r' && ++(*i) > 0)
        return '\r';
    else if (str[0] == '\\' && str[1] == 'e' && ++(*i) > 0)
        return '\033';
    return -1;
}

static int print_echo_e(char *str) {
    char buff;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\'
            && mx_regexp(str + i + 1, MX_REGULAR)) {
            buff = mx_0_and_x(str, &i);
        }
        else if(str[i] == '\\' && str[i + 1] == '\\')
            buff = '\\';
        else if (str[i] == '\\' && str[i + 1] == '0' && ++i > 0)
            buff = '\0';
        else if ((buff = print_echo_d(&str[i], &i)) != -1);
        else if (str[i] == '\\' && str[i + 1] == 'c' && ++i > 0)
            return 0;
        else
            buff = str[i];
        write(1, &buff, 1);
    }
    return 1;
}

static char *checkflags(char **str, int *counter) {
    char *flags = mx_strnew(2);

    flags[0] = '0';
    flags[1] = 'e';
    for (int i = 1; str[i]; i++) {
        if (str[i][0] != '-')
            return flags;
        for (int n = 1; str[i][n] != '\0'; n++)
            if (str[i][n] != 'n' && str[i][n] != 'e' && str[i][n] != 'E')
                return flags;
        *counter = i;
        for (int n = 1; str[i][n]; n++) {
            if (str[i][n] == 'n')
                flags[0] = 'n';
            else
                flags[1] = str[i][n] == 'e' ? 'e' : 'E';
        }
    }
    return flags;
}

static void print_e(int i, char *flags, char **str) {
    int error = 1;

    for (i = i + 1; str[i]; i++) {
        error = print_echo_e(str[i]);
        if (str[i + 1] && error)
            write(1, " ", 1);
    }
    if (flags[0] != 'n' && error)
        write(1, "\n", 1);
}

int mx_echo(char **argv, t_ush *ush) {

    int i = 0;
    char *flags = checkflags(argv, &i);

    if (flags[1] == 'E') {
        for (i = i + 1; argv[i]; i++) {
            write(1, argv[i], mx_strlen(argv[i]));
            if (argv[i + 1])
                write(1, " ", 1);
        }
        if (flags[0] != 'n')
            write(1, "\n", 1);
    }
    else {
        print_e(i, flags, argv);
    }
    ush->last_return = 0;
    free(flags);
    return 0;
}
