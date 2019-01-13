#include "common.h"



int createsocket(int *fd,struct context *ctx)

{

    struct sockaddr_in serveraddr;

    struct epoll_event ev;

    memset(&serveraddr,0x00,sizeof(struct sockaddr_in));



    serveraddr.sin_family = AF_INET;

    serveraddr.sin_port = htons(5555);

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);



    *fd = socket(AF_INET,SOCK_STREAM,0);

    if (*fd == -1)

    {

        printf("create socket error!\n");

        return -1;

    }



    if( bind(*fd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in)) == -1 )

    {

        printf("bind socket error!\n");

        return -1;

    }



    if(listen(*fd,5) == -1)

    {

        printf("listen error!\n");

        return -1;

    }



    if((ctx->epfd=epoll_create(1)) < 0)

    {

        printf("epoll_create error![%d][%s]\n ",errno,strerror(errno));

        return -1;

    }



    ev.data.fd = *fd;

    ev.events = EPOLLIN | EPOLLOUT;



    if(epoll_ctl(ctx->epfd,EPOLL_CTL_ADD,*fd,&ev) == -1)

    {

        printf("epoll_ctl error!\n");

        return -1;

    }



    return *fd;

}



int setnonblock(int fd)

{

    int flag = 0;

    int iRet;



    flag = fcntl(fd,F_GETFL);

    if (flag == -1)

    {

        printf("F_GETFL get error!\n");

        return -1;

    }



    flag |= O_NONBLOCK; /*set nonblock*/

    iRet = fcntl(fd,F_SETFL,flag);

    if( iRet == -1 )

    {

        printf("F_SETFL get error!\n");

        return -1;

    }

    return 0;

}



int non_iowrite(int fd,char *buf,int len)

{

    int left_len;

    int writed_pos = 0;



    left_len = len;

    while(left_len>0)

    {

        int writed_len = 0;

        writed_len = write(fd,buf+writed_pos,left_len);

        if(writed_len<=0)

        {

            if(errno == EAGAIN)

                writed_len = 0;

            else

                return -1;

        }

        left_len -= writed_len;

        writed_pos += writed_pos;

    }

    return len;



}



