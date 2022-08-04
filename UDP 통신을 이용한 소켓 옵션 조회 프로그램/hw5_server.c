//2017113724 ¼Õ¿øÁØ
//server
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
	int serv_sock;
	int sock, u_sock;
	int state;
	int snd_buf;
 	int len;

	SO_PACKET s;
	socklen_t clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

	if (serv_sock == -1) {
		printf("UDP socket creation error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		printf("bind() error!");
	}
	printf("Socket Option Server Start\n");
	len=sizeof(snd_buf);
	while (1) {
		clnt_adr_sz = sizeof(clnt_adr);
		recvfrom(serv_sock, &s, sizeof(SO_PACKET), 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		printf(">>> Received Socket Option : %s\n", s.BUF);

		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (s.number == 1) {
			state = getsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
		}
		else if (s.number == 2) {
			state = getsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, (void*)&snd_buf, &len);
		}
		else if (s.number == 3) {
			state = getsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&snd_buf, &len);
		}
		else if (s.number == 4) {
			state = getsockopt(serv_sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&snd_buf, &len);
		}
		else if (s.number == 5) {
			state = getsockopt(serv_sock, SOL_SOCKET, SO_BROADCAST, (void*)&snd_buf, &len);
		}
		else if (s.number == 6) {
			state = getsockopt(serv_sock, IPPROTO_IP , IP_TOS, (void*)&snd_buf, &len);
		}
		else if (s.number == 7) {
			state = getsockopt(serv_sock, IPPROTO_IP, IP_TTL, (void*)&snd_buf, &len);
		}
		else if (s.number == 8) {
			state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&snd_buf, &len);
		}
		else if (s.number == 9) {
			state = getsockopt(sock, IPPROTO_TCP, TCP_MAXSEG, (void*)&snd_buf, &len);
		}
		s.optval = snd_buf;
		s.result = state;
		printf("<<< Send option : %s : %d , result : %d\n", s.BUF, s.optval, s.result);
		

		sendto(serv_sock, &s, sizeof(SO_PACKET), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
	}

	close(serv_sock);
	close(sock);
	return 0;
}
