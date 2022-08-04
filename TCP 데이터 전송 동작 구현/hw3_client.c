//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define SEQ_START 1000

typedef struct{
	int seq; //SEQ number
	int ack; //ACK number
	int buf_len; //File read/write bytes
	char buf[BUF_SIZE];
}Packet;

int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	int str_len;

	Packet c;

	char file[BUF_SIZE];

	printf("Input file name : ");
	scanf("%s", file);
	printf("[Client] reqeust %s\n\n", file);

	FILE *fp=fopen(file, "w");

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//generate socket
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1){
		printf("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	//ask connection
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		printf("connect() error!");
	}

	//send file name
	write(sock, file, sizeof(file));
	char warning[BUF_SIZE];
	int total=0;
	read(sock, warning, sizeof(warning));
	
	if(strcmp(warning, "File Not Found")==0){
		printf("%s\n", warning);
		close(sock);
		exit(1);
	}
	while (1){
		read(sock, &c, sizeof(c));
		printf("[Client] Rx SEQ : %d, len:  %d bytes\n", c.seq, c.buf_len);
			
		total+=c.buf_len;
		c.ack=c.seq+c.buf_len+1;
		fwrite(c.buf, sizeof(char), c.buf_len, fp);
		write(sock, &c, sizeof(c));
		if (c.buf_len!=BUF_SIZE){
			printf("%s received (%d bytes)\n", file, total);
		}
		else{
			printf("[Client] Tx ACK : %d\n\n",c.ack);		
		}
		if (c.buf_len != BUF_SIZE){
			break;
		}
	
	}
	close(sock);
	return 0;
}
