#include "ush.h"

bool opencheck(char *dirname, t_cd *in) {
    DIR *dirdir;
    struct stat i;

    dirdir = opendir(dirname);
    if (errno == 13) {
        in->error = 4;
        return false;
    }
    if (errno == 2) {
        in->error = 1;
        return false;
    }
    if (dirdir == NULL) {
        in->error = 2;
        return false;
    }
    lstat(dirname, &i);
    if (MX_ISLNK(i.st_mode) && in->flag_s == 2) {
        in->error = 2;
        closedir(dirdir);
        return false;
    }
    closedir(dirdir);
    return true;
}
