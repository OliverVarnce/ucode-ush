#include "ush.h"

static bool cur_job(char *tmp , t_jobs **jobs){
    t_jobs *j = *jobs;
    
    if (mx_strcmp_null(tmp, "%") == 0 ||
        mx_strcmp_null(tmp, "+") == 0 || tmp == NULL) { 
        if (j->data == NULL && j->num == -1) {
            write (1, "fg: no current job\n", 19);
            return false;
        }
        else if(j->next == NULL) //голова
            del_job(&j, 1);
        else {
            while(j->next->next != NULL)
                j = j->next;
            del_job(&j, 2);
        }
    }
    return true;
}

static bool prev_job(char *tmp , t_jobs **jobs){
    t_jobs *j = *jobs;

    if (mx_strcmp_null(tmp, "-") == 0) { // previous job предпоследний
        if (j->next == NULL) {
            write (1, "fg: no previous job\n", 20);
            return false;
        }
        else if(j->next->next == NULL)
            del_job(jobs, 1);
        else {
            while(j->next->next->next != NULL)
                j = j->next;
            del_job(&j, 2);
        }
    }
    return true;
}

static bool name_job(char *tmp , t_jobs **jobs){
    t_jobs *j = *jobs;
    int num;

    if (mx_isalpha(tmp[0])) {
        num = name_search(tmp , j);
        if(num == -1) {
            not_found(tmp, "fg: job"); 
            return false;
        }
        else if (num == 0)
            del_job(&j, 1);
        else {
            while(num - 1){
                j = j->next;
                num--;
            }
            del_job(&j, 2);
        }
    }
    return true;
}

bool job_chars_find(char *args, t_jobs **jobs) {
    char *tmp = cut_str_forjob(args);
    bool flag = true;

    if (!cur_job(tmp, jobs))
        flag = false;
    if (!prev_job(tmp, jobs))
        flag = false;
    if (!name_job(tmp, jobs))
        flag = false;
    mx_strdel(&tmp);
    return flag;
}