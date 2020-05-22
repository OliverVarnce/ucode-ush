#include "ush.h"

void free_node(t_history *node) {
    free(node->data);
    free(node);
}
