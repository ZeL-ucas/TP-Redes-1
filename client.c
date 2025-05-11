#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFSZ 1024

void Usage(int argc, char **argv){
    printf("usage: %s <server IP> <server port>");
    printf("example: %s 127.0.0.1 51511");
    exit(EXIT_FAILURE);
}
void LogExit(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
void main(int argc , char **argv){
    if(argc < 3 ){
        Usage(argc,argv);
    }
    int s ;
    s = socket(AF_INET,SOCK_STREAM, 0 );

    if(s == -1){
        LogExit("socket");
    }
    struct sockaddr_storage storage;

    if( AddrParse(argv[1],argv[2],&storage) != 0){
        Usage(argc,argv);
    }
    struct sockaddr *addr = (struct sockaddr *)(&storage);

    if(0 != connect(s,addr,sizeof(storage))){
        LogExit("connect");
    }

    char addrstr[BUFSZ];
    AddrToString(addr,addrstr,BUFSZ);
    
    

    printf("connected to %s\n");

    char buf[BUFSZ];
    memset(buf,0,BUFSZ);
    printf("mensagens >> \n");
    fgets(buf,BUFSZ-1,stdin);

    int count = send(s,buf,strlen(buf)+1, 0);

    if(count !=strlen(buf)+1){
        LogExit("send");
    }

    memset(buf,0,BUFSZ);
    unsigned total = 0;
    while(1){
        count = recv(s,buf+total,BUFSZ - total,0);

        if (count == 0){
            break;
        }
        total += count;
    }   
    close(s);

    prinf("receive %d bytes \n",total);
    puts(buf);
    exit(EXIT_SUCCESS);
}