#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#define REMOTEIP "138.30.100.2"
#define REMOTEPORT 10210
#define LOCALPORT 10009

int totolNum = 0;

void main()
{
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    int rcv_num = -1;
    int lastPackFlag = 0;
    int sentLen = 0;
    unsigned char rcv_buff[16*1024];
    int sock_fd;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creat error");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(LOCALPORT);
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    printf("befor conn\r\n");

    if(bind(sock_fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("conn creat error\r\n");
        exit(1);
    }
    printf("bind ok\r\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(REMOTEPORT);
    server_addr.sin_addr.s_addr = inet_addr(REMOTEIP);
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("conn creat error\n");
        exit(1);
    }
    printf("after conn\r\n");
    while(1)
    {
        rcv_num = recv(sock_fd, rcv_buff, sizeof(rcv_buff), 0);
        if(rcv_num < 0)
        {
            printf("rcv error\n");
            break;
        }
        totolNum++;
        if(totolNum % 1000 ==0)
        {
            printf("rcv data %d\r\n", rcv_num);
        }
    }
    close(sock_fd);
    return;

}