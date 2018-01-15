#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <stdlib.h>
#include <signal.h>
#define SOCK_ADDR "/tmp/socket.domain"
#define SOCK_ERROR "/tmp/error.tex"
#define MAX_BUFFER_LEN 2046
FILE* stream;
void arlam(int sif)
{
    printf("sif=%d\n",sif);
}
int main()
{
    int socket_id=0;
    int cli_socket_id=0;
    char ret=0;
    struct sockaddr_un  ser_socket_st;
    struct sockaddr_un cli_socket_st;

    signal(SIGALRM, arlam);

    unlink(SOCK_ADDR);
    socket_id=socket(AF_UNIX,SOCK_STREAM,0);
    if(socket_id<0){
        stream=fopen(SOCK_ERROR,"w");
        fprintf(stream,"socket error:%s\n",strerror(errno));
        fclose(stream);
        printf("socket error: %s",strerror(errno));
        perror("socket: ");
        exit(0);
    }
    memset(&ser_socket_st,0,sizeof(ser_socket_st));
    strcpy(ser_socket_st.sun_path,SOCK_ADDR);
    ser_socket_st.sun_family=AF_UNIX;
   ret= bind(socket_id,(struct sockaddr*)&ser_socket_st,sizeof(ser_socket_st));
    if(ret==-1){
         perror("socket bind: ");
         close(socket_id);
         return -1;
    }
    ret=listen(socket_id,2);
    if(ret==-1){
        perror("listen error:");
        exit(1);
    }
    printf("whlie listeing-------------\n");
    memset(&cli_socket_st,0,sizeof(cli_socket_st));
    socklen_t  len=sizeof(cli_socket_st);
    cli_socket_id=accept(socket_id,(struct sockaddr*)&cli_socket_st,&len);
    if(cli_socket_id==-1){
        perror("accept error: ");
        exit(1);
    }
    while(1){
        int lenth=0;
        char data[MAX_BUFFER_LEN];
        memset(data,0,MAX_BUFFER_LEN);
        printf("socket is runing ped=[%d]\n",getpid());
        lenth=recv(cli_socket_id,data,MAX_BUFFER_LEN,0);
        if(lenth==-1){
            perror("recv error: ");
        }else if(lenth==0){
            printf("lenth==%d\n",lenth);
            alarm(5);
            pause();
        }else{
            printf("lenth==%d\n",lenth);
        }
        if(!fork()){
            printf("call sub process success --pid[%d]\n",getpid());
            printf("len=%d\n",lenth);
            printf("data=%s\n",data);
            strcpy(data,"have get your data");
            lenth=send(cli_socket_id,data,1024,0);
            if(lenth==-1){
                perror("send error: ");
            }
            exit(1);
        }else{
            printf("fpid=[%d]\n",getpid());
        }
    }
    printf("end end end \n");
    close(cli_socket_id);
    close(socket_id);
    unlink(SOCK_ADDR);
    return 1;
}








