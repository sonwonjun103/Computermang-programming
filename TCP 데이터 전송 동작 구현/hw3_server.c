//server
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
	int serv_sock;
	int clnt_sock;

	Packet s;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
	}

	//generate socket
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1){
		printf("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	//bind
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		printf("bind() error!");
	}

	//listen
	if(listen(serv_sock, 5)==-1){
		printf("listen() error!");
	}

	//connect with client
	clnt_addr_size=sizeof(clnt_addr);

	//accept
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock==-1){
		printf("accept() error!");
	}

	printf("-----------------------------------------\n");
	printf("    File Transmission Server             \n");
	printf("-----------------------------------------\n");

	//receive file name
	char file[BUF_SIZE];
	read(clnt_sock, file, sizeof(file));
	
	//open file
	FILE *fp=fopen(file, "r");
	char send[BUF_SIZE]={0,};
	s.seq=SEQ_START;
	s.ack=0;
	int total=0;
	if (fp==0){
		printf("%s File Not Found\n", file);
		char warning[BUF_SIZE]="File Not Found";
		write(clnt_sock, warning, sizeof(warning));
		close(clnt_sock);
		close(serv_sock);
		exit(1);
	}
	else if (fp){
		write(clnt_sock, send, sizeof(send));
		printf("[Server] sending %s\n\n", file);
		while(!feof(fp)){
			//read buf
			s.buf_len=fread(s.buf, sizeof(char), BUF_SIZE, fp);
			//send s
			total+=s.buf_len;
			write(clnt_sock, &s, sizeof(s));
			printf("[Server] Tx: SEQ: %d, %d byte data\n", s.seq, s.buf_len);

			//receive s
			read(clnt_sock, &s, sizeof(s));
			if (s.buf_len!=BUF_SIZE){
				printf("%s sent (%d bytes)\n", file, total);
			}
			else{
				printf("[Server] Rx ACK : %d\n\n", s.ack);
			}
			s.seq=s.ack;
			
		}
	}
	
	close(clnt_sock);
	close(serv_sock);
	fclose(fp);
	return 0;

}
