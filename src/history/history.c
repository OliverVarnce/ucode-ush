#include "ush.h"

static t_history *init_history(char *data) {
    t_history *history = (t_history *)malloc(sizeof(t_history) * 3);

    history->prev = NULL;
    history->next = NULL;
    history->data = mx_strdup(data);
    return history;
}

void mx_history_replenish(t_history **history, char *data) {
    t_history *story = *history;
    
    if (story == NULL)
        *history = init_history(data);
    else {
        story->prev = init_history(data);
        story->prev->next = story;
        story = story->prev;
        *history = story;
    } 
}
