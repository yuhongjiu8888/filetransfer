#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include <pthread.h>
#include "queue.h"

typedef struct  threadpool 
{
    pthread_cond_t m_cond;
    pthread_mutex_t m_lock;
    pthread_t *pid;
    queue_t *q;
    int size;
    int flag;
}threadpool_t;


/*创建线程池*/
threadpool_t* threadpool_init(int thread_num);

/*线程池添加任务*/
int threadpool_add_job(void *func(void *arg),void *arg,threadpool_t *p);

/*线程池销毁*/
int threadpool_destroy(threadpool_t *p);

/*线程任务*/
void *threadpool_task(void *threadpool );

#endif