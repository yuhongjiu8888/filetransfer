#include "common.h"

#include "threadpool.h"



threadpool_t* threadpool_init(int thread_num)

{

    threadpool_t *p = (threadpool_t *)malloc(sizeof(threadpool_t));

    if(p==NULL)
    {

        printf("threadpool_t malloc error\n");

        return NULL;

    }



    p->pid = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);

    if(p->pid==NULL)
    {

        printf("malloc pool->pid error\n");

        free(p);

        return NULL;

    }



    p->flag = 0;

    p->size = thread_num;



    pthread_mutex_init(&(p->m_lock),NULL);

    pthread_cond_init(&(p->m_cond),NULL);



    p->q = create_queue(QUEUE_NUM);

    if(p->q == NULL)
    {

        printf("queue create error!\n");

        free(p);

        return NULL;

    }



    int i,iRet;

    for(i=0; i<thread_num;i++)
    {

        iRet = pthread_create(&(p->pid[i]),NULL,threadpool_task,(void *)p);

        if(iRet < 0)
        {

            printf("pthread_create error!\n");

            free(p);

            return NULL;
        }

    }

    return p;

}



int threadpool_add_job(void *func(void *arg),void *arg, threadpool_t *p)
{
    task_t t;

    t.func = func;

    t.arg = arg;

    pthread_mutex_lock(&(p->m_lock));

    if(queue_tail_put(p->q,&t))
    {

        printf("add task success!\n");

    }
    pthread_cond_signal(&(p->m_cond));
    pthread_mutex_unlock(&(p->m_lock));
    return 0;

}



int threadpool_destroy(threadpool_t *p)
{

    if(p==NULL)
    {

        printf("threadpool is null!\n");

        return -1;
    }



    queue_destroy(p->q);



    int i;

    for(i=0;i<p->size;i++)
    {
        if(pthread_join(p->pid[i],NULL) != 0)
        {

            printf("pthread_join is error!\n");

            return -2;

        }

    }
    
    pthread_mutex_destroy(&(p->m_lock));

    pthread_cond_destroy(&(p->m_cond));

    free(p);



    return 0;

}



void *threadpool_task(void *threadpool)
{
	struct timeval now;
    struct timespec outtime;
	int  timeout = 3000;
    threadpool_t *p = (threadpool_t *)threadpool;

    for(;;)
    {
        pthread_mutex_lock(&(p->m_lock));
        if (queue_is_empty(p->q))
        {
           
            gettimeofday(&now, NULL);
            if (now.tv_usec + timeout > 1000)
            {
                outtime.tv_sec = now.tv_sec + 1;
                outtime.tv_nsec = ((now.tv_usec + timeout) % 1000) * 1000;
            }
            else
            {
                outtime.tv_sec = now.tv_sec;
                outtime.tv_nsec = (now.tv_usec + timeout) * 1000;
            }
            pthread_cond_timedwait(&(p->m_cond), &(p->m_lock), &outtime);
        }
        task_t *t = queue_head_get(p->q);
        if(t != NULL)
        {

            t->func(t->arg);

        }



        if( (p->flag == 1) && (queue_is_empty(p->q)))
        {

            printf("thread [%ld] is exit.\n",pthread_self());

            break;

        }
        pthread_mutex_unlock(&(p->m_lock));
    }

    return NULL;

}
