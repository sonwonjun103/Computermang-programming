//2017113724 ¼Õ¿øÁØ
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

void * send_f(void * arg);
void * recv_f(void * arg); 
	
#define BUF_SIZE 100

static sem_t sem_one;
static sem_t sem_two;

#define NUM 20

typedef struct{
    int command;
    int seatno;
    int seats[NUM];
    int result;
}BUS_INFO;

BUS_INFO clnt;
pthread_mutex_t mutex;

void show();
void inquiry(int sock);
void reserve(int sock);
void cancel(int sock);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t send, recv;
	void * thread_return;
	if(argc!=3) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        printf("connect() error\n");
    }

	write(sock, argv[1], sizeof(argv[1]));
	
	pthread_create(&send, NULL, send_f, (void*)&sock);
	pthread_create(&recv, NULL, recv_f, (void*)&sock);

	pthread_join(send, NULL);
	pthread_join(recv, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	
	return 0;
}

void * send_f(void * arg) {
	int sock=*((int*)arg);
	
	while (1){
		sem_wait(&sem_two);
		int option;
	
		printf("1 : inquiry, 2 : reservation, 3 : cancellation, 4 : quit : ");
		scanf("%d", &option);
		clnt.command=option;
		if(option==1){
			inquiry(sock);
		}
		else if(option==2){
			reserve(sock);
		}
		else if(option==3){
			cancel(sock);
		}
		else if(option==4){
			printf("Quit\n");
			close(sock);
			break;
		}
		sem_post(&sem_one);
	}
	
	return NULL;
}

void * recv_f(void * arg){
    int sock=*((int*)arg);
	
    while (1){
		sem_wait(&sem_one);
        read(sock, &clnt, sizeof(clnt));	
		show(clnt);
		if (clnt.result==0){
			printf("Operation success\n");
		}
		else if(clnt.result==-1){
			printf("Wrong seat number\n");
		}
		else if(clnt.result==-2){
			printf("Reservation failed.(The seat was already reserved)\n");
		}
		else if(clnt.result==-3){
			printf("Cancellation failed(The seat was not reserved)\n");
		}
		else if(clnt.result==-4){
			printf("Cancllation failed. (The seat was reserved by another person)\n");
		}
		sem_post(&sem_two);
	}
	
	return NULL;
}

void show(){
	printf("--------------------------------------\n");
	for(int i=0;i<10;i++){
		printf("%d ", i+1);
	}
	printf("\n");
	for(int i=0;i<10;i++){
		printf("%d ", clnt.seats[i]);
	}
	printf("\n");
	printf("--------------------------------------\n");
	for(int i=10;i<NUM;i++){
		printf("%d ", i+1);
	}
	printf("\n");
	for(int i=10;i<NUM;i++){
		printf("%d ", clnt.seats[i]);
	}
	printf("\n--------------------------------------\n");
}

void inquiry(int sock){
	write(sock, &clnt, sizeof(clnt));
}

void reserve(int sock){
	int seatno;
	printf("Input seat number : ");
	pthread_mutex_lock(&mutex);
	scanf("%d", &seatno);
	clnt.seatno=seatno;
	pthread_mutex_unlock(&mutex);
	write(sock, &clnt, sizeof(clnt));
}

void cancel(int sock){
	int cancel;
	printf("Input seat number for cancellation : ");
	pthread_mutex_lock(&mutex);
	scanf("%d", &cancel);
	clnt.seatno=cancel;
	pthread_mutex_unlock(&mutex);
	write(sock, &clnt, sizeof(clnt));
}