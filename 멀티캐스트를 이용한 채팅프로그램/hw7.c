//2017113724 ¼Õ¿øÁØ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 60

int main(int argc, char *argv[]){
    int send_sock;
    int recv_sock;
    struct sockaddr_in mul_adr;
    int time_live=TTL;
    char buf[BUF_SIZE], message[BUF_SIZE];
    struct ip_mreq join_adr;
    int check=1;

    if(argc!=4){
        printf("Usage : %s <GroupIP> <PORT> <Name>\n", argv[0]);
        exit(1);
    }

    send_sock=socket(AF_INET, SOCK_DGRAM, 0);
    recv_sock=socket(AF_INET, SOCK_DGRAM, 0);

    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family=AF_INET;
    mul_adr.sin_addr.s_addr=inet_addr(argv[1]); //multicast IP
    mul_adr.sin_port=htons(atoi(argv[2])); //multicast port

    join_adr.imr_multiaddr.s_addr=inet_addr(argv[1]);
    join_adr.imr_interface.s_addr=htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void*)&join_adr, sizeof(join_adr));

    setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR,&check,sizeof(check));
    

    if(bind(recv_sock, (struct sockaddr*)&mul_adr, sizeof(mul_adr))==-1){
        printf("bind() error\n");
    }

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live)); //time-to-live(TTL)

    pid_t pid;
    pid=fork();

    if(pid==0){
        //child
        while (1){
            int str_len=recvfrom(recv_sock, message, BUF_SIZE-1, 0, NULL, 0);
            int str_len_name=recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
            message[str_len]=0;
            buf[str_len_name]=0;
            printf("Received Message : [%s] %s", buf, message);
        }
        close(recv_sock);
    }
    else{
        //parent
        while (1){
            fgets(message, BUF_SIZE, stdin);

            sendto(send_sock, message, strlen(message), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
            sendto(send_sock, argv[3], strlen(argv[3]), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        }
        close(send_sock);
    }
    return 0;
}