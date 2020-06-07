#include "ush.h"

void mx_free_node(t_history *node) {
    free(node->data);
    free(node);
}
