#include "ush.h"

void free_node(t_hst *node) {
    free(node->data);
    free(node);
}
