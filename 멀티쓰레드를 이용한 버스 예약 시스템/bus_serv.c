//2017113724 ¼Õ¿øÁØ
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define NUM 20
#define MAX_CLNT 256

typedef struct{
    int command;
    int seatno;
    int seats[NUM];
    int result;
}BUS_INFO;

BUS_INFO serv;
pthread_mutex_t mutex;

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];

void * handle_clnt(void*arg);
void send_to();

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1){
        printf("bind() error\n");
    }

    if(listen(serv_sock, 5)==-1){
        printf("listen() error\n");
    }

    printf("Bus Reservation System\n");

    char IP[MAX_CLNT];

	while(1)
	{
        clnt_adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);

        int IP_len=read(clnt_sock, IP, sizeof(IP));
        IP[IP_len]=0;
        printf("Connected client IP : %s, clnt_sock=%d\n", IP, clnt_sock);

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++]=clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
    }

    pthread_mutex_destroy(&mutex);
    close(serv_sock);
    return 0;
}

void * handle_clnt(void * arg){
    int clnt_sock=*((int*)arg);
    int str_len=0;

    // 
    while ((str_len=read(clnt_sock, &serv, sizeof(serv)))!=0){
        
        if (serv.command==1){
            // pthread_mutex_lock(&mutex);
            serv.result=0;
            // pthread_mutex_unlock(&mutex);
        }
        else if(serv.command==2){
            // pthread_mutex_lock(&mutex);
            if(serv.seatno>20){
                serv.result=-1;
            }
            else if(serv.seats[serv.seatno-1]!=0){
                serv.result=-2;
            }
            else{
                serv.result=0;
                serv.seats[serv.seatno-1]=clnt_sock;
            }
            // pthread_mutex_unlock(&mutex);
        }
        else if(serv.command==3){
            // pthread_mutex_lock(&mutex);
            if(serv.seats[serv.seatno-1]==0){
                serv.result=-3;
            }
            else if(serv.seats[serv.seatno-1]!=clnt_sock){
                serv.result=-4;
            }
            else{
                serv.result=0;
                serv.seats[serv.seatno-1]=0;
            }
            // pthread_mutex_unlock(&mutex);
        }
        else if(serv.command==4){
            // pthread_mutex_lock(&mutex);
            for (int i = 0; i < clnt_cnt; i++)
            {
                if (clnt_sock == clnt_socks[i])
                {
                    while (i < clnt_cnt)
                        clnt_socks[i] = clnt_socks[i + 1];
                    i++;
                    break;
                }
            }
            clnt_cnt--;
            // pthread_mutex_unlock(&mutex);
            close(clnt_sock);
        }
        send_to();
    }
    // pthread_mutex_unlock(&mutex);

    return NULL;
}

void send_to(){
    int i;
    pthread_mutex_lock(&mutex);
    for(i=0;i<clnt_cnt;i++){
        write(clnt_socks[i], &serv, sizeof(serv));
    }
    pthread_mutex_unlock(&mutex);
}