#include "libmx.h"

static void swap_elements(char **left, char **right) {
    char *temp = *left;
    *left = *right;
    *right = temp;
}

int mx_quicksort(char **arr, int left, int right) {
    if (!arr) return -1;
    if (left >= right) return 0;

    int swaps = 0;
    int pi = left;
    char *pivot = arr[left+(right-left)/2];
    int pivot_len = mx_strlen(pivot);

    for (int r = right; pi <= r;) {
        for (; mx_strlen(arr[pi]) < pivot_len ; pi++);
        for (; mx_strlen(arr[r]) > pivot_len ; r--);

        if (pi <= r) {
            if (mx_strlen(arr[pi]) != mx_strlen(arr[r])) {
                swap_elements(&arr[pi], &arr[r]);
                swaps++;
            }
            pi++;
            r--;
        }
    }
    return swaps + mx_quicksort(arr, left, pi - 1) + mx_quicksort(arr, pi, right);
}
