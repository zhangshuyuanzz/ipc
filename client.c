#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <stdlib.h>

#define SOCK_ADDR "/tmp/socket.domain"
#define SOCK_ERROR "/tmp/error.tex"
#define MAX_BUFFER_LEN 2046

int main()
{
    char ret=0;
    int cli_socket_id=0;
    char buffer[1024];
    struct sockaddr_un cli_addr;
    memset(&cli_addr,0,sizeof(cli_addr));
    cli_socket_id=socket(AF_UNIX,SOCK_STREAM,0);
    if(cli_socket_id<0){
        perror(" client socket error: ");
        exit(1);
    }
    cli_addr.sun_family=AF_UNIX;
    strcpy(cli_addr.sun_path,SOCK_ADDR);
    ret=connect(cli_socket_id,(struct sockaddr*)&cli_addr,sizeof(cli_addr));
    if(ret==-1){
        perror("cli connect fial:");
        close(cli_socket_id);
        exit(1);
    }
    if(!fork()){
        while(1){
            memset(buffer,0,1024);
            recv(cli_socket_id,buffer,1024,0);
            printf("cli get data:%s\n",buffer);
        }
    }
    while(1){
        memset(buffer,0,1024);

        printf("input string:\n");
        fgets(buffer,1024,stdin);
        printf("string=%s\n",buffer);
        ret=send(cli_socket_id,buffer,strlen(buffer),0);
        if(ret==-1){
            perror("cli send error:");
            exit(1);
        }

    }
    close(cli_socket_id);
}
