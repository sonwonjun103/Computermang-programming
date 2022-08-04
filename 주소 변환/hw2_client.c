#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
	while (1)
	{
		int sock;
		struct sockaddr_in serv_addr;
		char message[30];
		int str_len;
		if(argc!=3)
		{
			printf("usage : %s <IP> <port>\n", argv[0]);
			exit(1);
		}
		
		printf("Input dotted-decimal address : ");
		scanf("%s", message);
		sock=socket(PF_INET, SOCK_STREAM, 0);
		if(sock==-1)
		{
			printf("socket() error!\n");
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
		serv_addr.sin_port=htons(atoi(argv[2]));

		//connnect
		if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		{
			printf("connect() error!\n");
		}
			
		write(sock, message, sizeof(message));
		if (strcmp(message, "quit")==0)
		{
			printf("quit\n");
			break;
		}
		printf("Address conversion success\n\n");
		close(sock);	
	}
	return 0;
}
