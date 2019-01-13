#ifndef _COMMOM_H

#define _COMMOM_H



#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include <sys/socket.h>

#include <sys/epoll.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <fcntl.h>

#include <arpa/inet.h>

#include <errno.h>



#define EFDNUM 10

#define QUEUE_NUM 1024

#define THREAD_NUM 10

#define MAX_BUFFER_LENGTH		1024
#define MAX_FILE_LENGTH			512*1024
#define MAX_EPOLLSIZE			512
#define MAX_CLIENTS_COUNT		1024




typedef struct context{

    int epfd;

    struct epoll_event *ev;

    int clifd;

}ctx;

/* ** **** ******** **************** protocol start **************** ******** **** ** */

#define CLIENT_ID_LENGTH		4

#define FILE_PROTO_VERSION			0
#define FILE_PROTO_CMD				1
#define FILE_PROTO_SELF_ID			2
#define FILE_PROTO_PACKET_IDX		(FILE_PROTO_SELF_ID+CLIENT_ID_LENGTH)  //6
#define FILE_PROTO_PACKET_COUNT		(FILE_PROTO_PACKET_IDX+2) //8
#define FILE_PROTO_PACKET_LENGTH	(FILE_PROTO_PACKET_COUNT+2) //10
#define FILE_PROTO_PACKET_TYPE		(FILE_PROTO_PACKET_LENGTH+2) //10


#define FILE_PACKET_CRC_VALUE		0x5A5AA5A5

#define FILE_PACKET_HEADER			16
#define FILE_PACKET_TAIL			4

#define FILE_VERSION_0				0x0
#define FILE_CMD_BINARY				'B'


#define FILE_TYPE_JPG				0x00
#define FILE_TYPE_MP3				0x01


/* ** **** ******** **************** protocol end **************** ******** **** ** */



int write_file(char *filename, char *buffer, int length, char isappend);
int read_file(char *filename, char *buffer, int length);
int send_buffer(int sockfd, char *buffer, int length);
int recv_buffer(int sockfd, char *buffer, int length, int *ret); 
int decode_packet(char *buffer, int *length,  int *selfid, unsigned short *idx, unsigned short *count, char *type);
void*  async_works(void *arg);

struct context * initParameter();
int run(int fd,struct context *ctx);
int createsocket(int *fd,struct context *ctx);


#endif
