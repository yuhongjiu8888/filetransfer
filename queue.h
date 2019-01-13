#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef __cplusplus
typedef char bool;
#define false 0
#define true  1
#endif

typedef 
struct task
{
    void* (*func)(void *arg);
    void *arg;
}task_t;

typedef struct queue{
    int head;
    int tail;
    int size;
    int capital;
    task_t *job;
}queue_t;


/*创建队列*/
queue_t* create_queue(int size);

/*队列是否为空*/
bool queue_is_empty(queue_t *q);

/*队列是否为满*/
bool queue_is_full(queue_t *q);

/*队列尾部添加任务*/
bool queue_tail_put(queue_t *q, task_t *job);

/*队列头部取任务*/
task_t* queue_head_get(queue_t *q);

/*销毁队列*/
void queue_destroy(queue_t *q);


#endif
