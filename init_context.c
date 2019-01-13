#include "common.h"
#include "threadpool.h"



struct context * initParameter()

{

    struct context *ct = (struct context *)malloc(sizeof(struct context));

    if(ct==NULL)
    {

        printf("malloc context error!\n");

        return NULL;

    }

    return ct;

}



int run(int fd,struct context *ctx)

{

    printf("receive waiting .......\n");

    int nread,i;

    ctx->ev = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EFDNUM);

    if(NULL == ctx->ev)

    {

        printf("malloc ctx->ev error!\n");

        return -1;

    }

    /*创建线程池*/
    threadpool_t* pool = threadpool_init(THREAD_NUM);
    if(NULL == pool)
    {
        printf("create threadpool error!\n");
        return -2;
    }  

    while(1)
    {

        nread = epoll_wait(ctx->epfd,ctx->ev,EFDNUM,3000);

        if(nread<0)
        {

            if(errno==EAGAIN)

            {
                printf("EAGAIN!\n");
                continue;

            }

            else

            {

                printf("epoll_wait is error!\n");

                return -1;

            }

            

        }
        else{
            for(i=0 ;i<nread; i++)
            {
                if ((ctx->ev[i].events & EPOLLERR) || (ctx->ev[i].events & EPOLLHUP)
                || !(ctx->ev[i].events & EPOLLIN))
                {
                    perror("error : socket fd error!\n");
                    close(ctx->ev[i].data.fd);
                    continue;
            
                }
                else if(ctx->ev[i].data.fd==fd)  /*new client connect server*/
                {

                    struct sockaddr_in clientaddr;

                    int clientfd;

                    memset(&clientaddr,0x00,sizeof(struct sockaddr_in));

                    socklen_t len = sizeof(clientaddr);

                    clientfd = accept(fd,(struct sockaddr*)&clientaddr,&len);

                    if(clientfd == -1)
                    {

                        printf("accept error!\n");

                        return -1;

                    }

                    printf("new client come from [%s][%d]\n",

                    inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));



                    struct epoll_event ev;

                    ev.events = EPOLLIN;

                    ev.data.fd = clientfd;

                    ctx->clifd = clientfd;

                    if((epoll_ctl(ctx->epfd,EPOLL_CTL_ADD,clientfd,&ev))==-1)
                    {

                        printf("epoll_ctl clientfd error!\n");

                        exit(-1);

                    }

                }
                else{

                    threadpool_add_job(async_works,(void *)ctx,pool);/*丢到线程池*/

                }

            }

        }

    }

    return 0;

}

