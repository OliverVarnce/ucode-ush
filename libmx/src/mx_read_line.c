#include "libmx.h"

static void add_to_line(char **lineptr, char *fragment, int fr_len) {
    int ptr_len = mx_strlen(*lineptr);
    char *temp = NULL;

    if (*lineptr) {
        temp = mx_strncat(mx_strcpy(mx_strnew(ptr_len + fr_len), *lineptr),
                         fragment, fr_len);
        free(*lineptr);
    } else {
        temp = mx_strndup(fragment, fr_len);
    }
    *lineptr = temp;
}

static bool read_from_buffer(char **lineptr, char delim, const int fd,
                             char **static_buf, int *total_bytes) {
    int di = 0;
    int buf_len = 0;

    if (!static_buf[fd])
        return false;

    di = mx_get_char_index(static_buf[fd], delim);
    buf_len = mx_strlen(static_buf[fd]);

    if (di >= 0) {
        char *temp = NULL;

        // add fragment until delimeter to line:
        add_to_line(lineptr, static_buf[fd], di);

        // reduce static buffer by fragment length:
        temp = mx_strndup(&static_buf[fd][di+1], buf_len - (di+1));

        free(static_buf[fd]);
        static_buf[fd] = temp;
        *total_bytes += di;

        return true;
    }

    // if delimeter not found then add whole static buffer to line:
    add_to_line(lineptr, static_buf[fd], buf_len);
    *total_bytes += buf_len;
    free(static_buf[fd]);
    static_buf[fd] = NULL;

    return false;
}

static bool read_from_fd(char **lineptr, size_t buf_size, char delim,
                         const int fd, char **static_buf, int *total_bytes) {

    char *buf = mx_strnew(buf_size-1);
    int br = 0;
    int di = 0;

    if (!buf)
        return -2;

    while ((br = read(fd, buf, buf_size)) > 0) {
        if ((di = mx_get_char_index(buf, delim)) >= 0) {
            add_to_line(lineptr, buf, di);
            static_buf[fd] = mx_strndup(&buf[di+1], br - (di+1));
            free(buf);
            *total_bytes += di;
            return true;
        }
        add_to_line(lineptr, buf, br);
        *total_bytes += br;
        mx_memset(buf, '\0', buf_size);
    }
    free(buf);
    return false;
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    static char *static_buf[4096];
    int total_bytes = 0;

    if (fd < 0 || buf_size <= 0)
        return -2;

    if (read_from_buffer(lineptr, delim, fd, static_buf, &total_bytes) ||
        read_from_fd(lineptr, buf_size, delim, fd, static_buf, &total_bytes)) {

        return total_bytes;
    }

    return -1;
}
