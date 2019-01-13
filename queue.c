#include "queue.h"
queue_t* create_queue(int queue_size)
{
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    if(NULL == q)
    {
        printf("malloc queue_t is error!\n");
        return NULL;
    }

    q->head = q->tail = q->size = 0;
    q->capital = queue_size;
    q->job = (task_t*)malloc(queue_size * sizeof(task_t));
    if(q->job == NULL)
    {
        printf("queue job malloc error!\n");
        free(q);
        return NULL;
    }
    return q;
}

bool queue_is_empty(queue_t *q)
{
    return (q->size == 0);
}

bool queue_is_full(queue_t *q)
{
    return (q->size == q->capital);
}

bool queue_tail_put(queue_t *q,task_t *job)
{
    if( NULL == job )
    {
        printf("task_t job is NULL \n");
        return true;
    }

    if(!queue_is_full(q))
    {
        q->job[q->tail].func = job->func;
        q->job[q->tail].arg = job->arg;
        q->tail = (q->tail+1)%(q->capital);
        q->size++;
        return true;
    }
    return false;
}

task_t* queue_head_get(queue_t *q)
{
    task_t *t = NULL;
    if(!queue_is_empty(q))
    {
        t = &q->job[q->head];
        q->head = (q->head+1)%(q->capital);
        q->size--;
    }
    return t;
}

void queue_destroy(queue_t *q)
{
    if(q==NULL)
        return;
    if(q->job == NULL)
    {
        free(q);
        return ;
    }

    free(q->job);
    free(q);
    return ;
}
