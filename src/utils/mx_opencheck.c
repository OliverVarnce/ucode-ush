#include "ush.h"

bool mx_opencheck(char *dirname, t_cd *in) {
    DIR *dir;
    struct stat i;

    dir = opendir(dirname);
    if (errno == 2) {
        in->error = 1;
        return false;
    }
    if (errno == 13) {
        in->error = 4;
        return false;
    }
    if (dir == NULL) {
        in->error = 2;
        return false;
    }
    lstat(dirname, &i);
    if (MX_ISLNK(i.st_mode) && in->flag_s == 2) {
        in->error = 2;
        closedir(dir);
        return false;
    }
    closedir(dir);
    return true;
}
