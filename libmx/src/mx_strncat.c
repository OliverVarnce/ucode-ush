#include "libmx.h"

char *mx_strncat(char *restrict s1, const char *restrict s2, size_t n) {
    int i = mx_strlen(s1);

    for (int j = 0; s2[j] != '\0' && n--; i++, j++) {
        s1[i] = s2[j];
    }
    s1[i] = '\0';

    return s1;
}