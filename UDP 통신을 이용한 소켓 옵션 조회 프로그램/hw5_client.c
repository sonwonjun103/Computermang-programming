//2017113724 ¼Õ¿øÁØ
//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#define BUF_SIZE 20

typedef struct{
	char BUF[BUF_SIZE];
	int number;
	int optval;
	int result;
}SO_PACKET;

int main(int argc, char* argv[]) {
	int number;
	int sock;
	SO_PACKET c;

	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		printf("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while (1) {
		printf("------------------------------\n");
		printf("1. SO_SNDBUF\n");
		printf("2. SO_RCVBUF\n");
		printf("3. SO_REUSEADDR\n");
		printf("4. SO_KEEPALIVE\n");
		printf("5. SO_BROADCAST\n");
		printf("6. IP_TOS\n");
		printf("7. IP_TTL\n");
		printf("8. TCP_NODELAY\n");
		printf("9. TCP_MAXSEG\n");
		printf("10. Quit\n");
		printf("------------------------------\n");

		printf("Input option number : ");
		scanf("%d", &number);

		if (number == 10) {
			close(sock);
			printf("Client quit\n");
			break;
		}
		else if (0 <= number && number > 10) {
			printf("Wrong number. type again!\n");
		}
		else {
			memset(&c, 0, sizeof(SO_PACKET));
			c.number = number;
			if (number == 1) {
				strcpy(c.BUF, "SO_SNDBUF");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 2) {
				strcpy(c.BUF, "SO_RCVBUF");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 3) {
				strcpy(c.BUF, "SO_REUSEADDR");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 4) {
				strcpy(c.BUF, "SO_KEEPALIVE");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 5) {
				strcpy(c.BUF, "SO_BROADCAST");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 6) {
				strcpy(c.BUF, "IP_TOS");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 7) {
				strcpy(c.BUF, "IP_TTL");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 8) {
				strcpy(c.BUF, "TCP_NODELAY");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
			else if (number == 9) {
				strcpy(c.BUF, "TCP_MAXSEG");
				sendto(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
				adr_sz = sizeof(from_adr);
				recvfrom(sock, &c, sizeof(SO_PACKET), 0, (struct sockaddr*)&from_adr, &adr_sz);
				printf(">>> Server result : %s : value : %d, result : %d\n", c.BUF, c.optval, c.result);
			}
		}
	}

	return 0;
}

					





