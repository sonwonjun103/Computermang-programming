#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
	printf("----------------------------------------\n");
	printf("  Address conversion server             \n");
	printf("----------------------------------------\n");
	while (1)
	{
		int serv_sock;
		int clnt_sock;
		
		char message[30];
		struct sockaddr_in serv_addr;
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_size;

		serv_sock=socket(PF_INET, SOCK_STREAM, 0);
		if(serv_sock==-1)
		{
			printf("socket() error\n");
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		serv_addr.sin_port=htons(atoi(argv[1]));

		if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		{
			printf("bind() error\n");
		}

		if(listen(serv_sock, 5)==-1)
		{
			printf("listen() error\n");
		}

		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

		if(clnt_sock==-1)
		{
			printf("accept() error\n");
		}
		
		int str_len;
		str_len=read(clnt_sock, message, sizeof(message)-1);
		if(str_len==-1)
		{
			printf("read() error!");
		}
		else
		{
			printf("Received Dotted-Decimal Address : %s\n", message);
			
			struct sockaddr_in addr_inet;
			if(strcmp(message, "quit")==0)
			{
				printf("quit received and exit program!\n");
				break;
			}
			else if(!inet_aton(message, &addr_inet.sin_addr))
			{
				printf("Address converesion fail : Format error!\n\n");
			}
			else
			{
				printf("inet_aton : %s -> %#x\n", message, addr_inet.sin_addr.s_addr);
			
				struct sockaddr_in addr1;
				char* str_ptr;
				addr1.sin_addr.s_addr=htonl(addr_inet.sin_addr.s_addr);
				str_ptr=inet_ntoa(addr1.sin_addr);
				printf("inet_ntoa : %#x -> %s\n", addr_inet.sin_addr.s_addr, str_ptr);
				printf("Address conversion success\n\n");
			}
		}
		close(serv_sock);
		close(clnt_sock);
	
	}
	return 0;
}
