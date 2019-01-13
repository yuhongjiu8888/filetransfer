#include "common.h"

int write_file(char *filename, char *buffer, int length, char isappend) {
    FILE *fp = NULL;

    if (isappend) {
        fp = fopen(filename, "ab+");
    } else {
        fp = fopen(filename, "wb+");
    }

    if (fp == NULL) {
        printf("fopen failed\n");
        return -1;
    }

    int size = fwrite(buffer, 1, length, fp);
    if (size != length) {
        printf("fread failed count : %d\n", size);
        goto _exit;
    }

_exit:
    fclose(fp);
    return size;
}

int read_file(char *filename, char *buffer, int length) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("fopen failed\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int count = ftell(fp);
    if (count > length) {
        printf(" %s file too big\n", filename);
        fclose(fp);
        return -2;
    }
	
    fseek(fp, 0, SEEK_SET);
    int size = fread(buffer, 1, count, fp);
    if (size != count) {
        printf("fread failed count : %d\n", size);
        goto _exit;
    }

_exit:
    fclose(fp);
    return size;
}

int send_buffer(int sockfd, char *buffer, int length) {
    int idx = 0;
    while (idx < length) {
        int count = 0;
        if ((idx + MAX_BUFFER_LENGTH) < length) {
            count = send(sockfd, buffer+idx, MAX_BUFFER_LENGTH, 0);
        } else {
            count = send(sockfd, buffer+idx, length-idx, 0);
        }

    if (count <= 0) break;
    idx += count;
    }

    return idx;
}

int recv_buffer(int sockfd, char *buffer, int length, int *ret) 
{
    int idx = 0;
    while (1) {
        int count = recv(sockfd, buffer+idx, length - idx, 0);
        if (count == 0) {
            *ret = -1;
            close(sockfd);
            break;
        } else if (count == -1) {
            printf("recv success --> count : %d\n", idx);
            *ret = 0;
            break;
        } else {			
            idx += count;
            if (idx == length) break;
        }
    }

    return idx;
}

int decode_packet(char *buffer, int *length,  int *selfid, unsigned short *idx, unsigned short *count, char *type) 
{
    *selfid = *(int *)(buffer + FILE_PROTO_SELF_ID);
    *idx = *(unsigned short *)(buffer + FILE_PROTO_PACKET_IDX);
	
    *count = *(unsigned short*)(buffer + FILE_PROTO_PACKET_COUNT);
    *length = *(unsigned short*)(buffer + FILE_PROTO_PACKET_LENGTH);

    *type = *(buffer + FILE_PROTO_PACKET_TYPE);

    printf(" decode_packet --> idx:%d, count:%d, selfid:%d\n", *idx, *count, *selfid);

    if (*idx == *count-1) return 1;

    return 0;
}

void*  async_works(void *arg)
{
    ctx *t = (ctx *)arg;
    if( t==NULL )
    {
        printf("context is null!\n");
        return NULL;
    }
    char *data = (char*)malloc(MAX_FILE_LENGTH * sizeof(char));
    if (data == NULL) {
         printf("malloc failed\n");
    }

    struct epoll_event ev;
    int ret = 0;
    int length = recv_buffer(t->clifd, data, MAX_FILE_LENGTH, &ret);
    if (ret < 0) {
					
         ev.events = EPOLLIN | EPOLLET;
         ev.data.fd = t->clifd;
         epoll_ctl(t->epfd, EPOLL_CTL_DEL, t->clifd, &ev);

         printf("length : %d\n", length);
         int count = write_file("a.txt", data, length, 1);
         if (count < 0) {
            printf("write_file failed\n");
         }			
    }

}
