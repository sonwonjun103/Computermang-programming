//2017113724 ¼Õ¿øÁØ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 2048

int main(int argc, char *argv[])
{
	int fd1,fd2;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
    struct timeval timeout;
    fd_set cp_reads, reads;
    int fd_num, fd_max;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

    fd1=open("rfc1180.txt", O_RDONLY,0644);
	
	fd2=socket(PF_INET, SOCK_STREAM, 0);   
	if(fd2==-1)
		printf("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

    if(connect(fd2, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1){
        printf("connect() error!\n");
    }

    FD_ZERO(&reads);
  
    FD_SET(fd1, &reads);
    FD_SET(fd2, &reads);

    if (fd1>fd2){
        fd_max=fd1;
    }
    else{
        fd_max=fd2;
    }

    printf("Connected......\n");
    while (1){
        cp_reads=reads;

        timeout.tv_sec=3;
        timeout.tv_usec=0;

        fd_num=select(fd_max+1,&cp_reads, 0, 0, &timeout);
      
        if(fd_num==-1){
            break;
        }
        else if(fd_num==0){
            continue;
        }
        else{
            //if file open, transfer content to server
            if(FD_ISSET(fd1, &cp_reads)){
                str_len=read(fd1, message, BUF_SIZE);
                write(fd2, message, str_len);
                sleep(1);
                FD_CLR(fd1, &cp_reads);
            }

            //if connecting success, receive content which is sent by server
            if(FD_ISSET(fd2, &cp_reads)){

                int read_len=read(fd2, message, sizeof(message));
                message[read_len]=0;
                printf("%s\n", message);

                FD_CLR(fd2, &cp_reads);
            }
        }
    }
    close(fd2);
    return 0;
}