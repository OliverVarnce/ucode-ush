#include "ush.h"

void mx_pop_front_proc(t_processes **head) {
    if (head == NULL || *head == NULL)
        return;

    t_processes *temp = *head;
    *head = temp->next;
    free(temp);
}
