//2017113724 손원준
//client
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BOARD_SIZE 3
#define S_VALUE 1
#define C_VALUE 2
#define INIT_VALUE 0

typedef struct{
	int board[BOARD_SIZE][BOARD_SIZE];
}GAMEBOARD;

int available_space(GAMEBOARD* gboard);
void draw_board(GAMEBOARD* gboard);

int main(int argc, char *argv[]){
	int sock;
	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;
	int row, column;

	GAMEBOARD c;
	
	if(argc!=3){
		printf("Usage : %s <port>\n", argv[0]);
	}

	printf("Tic-Tac-Toe Client\n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			c.board[i][j] = 0;
		}
	}

	//generate socket
	sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(sock==-1){
		printf("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	draw_board(&c);
	while (1) {
		printf("Input row, column : ");
		scanf("%d %d", &row, &column);
		if (row < BOARD_SIZE && column < BOARD_SIZE && c.board[row][column] == 0) {	
			c.board[row][column] = C_VALUE;
			draw_board(&c);
			sendto(sock, c.board, sizeof(GAMEBOARD), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

			//공간 체크
			if(available_space(&c)!=0){
				//공간이 남으면 서버로부터 받기
				adr_sz = sizeof(from_adr);
				recvfrom(sock, c.board, sizeof(GAMEBOARD), 0, (struct sockaddr*)&from_adr, &adr_sz);
				draw_board(&c);
			}
			else{
				//공간이 없으면 끝내기
				printf("No available space. Exit Client.\n");
				printf("Tic Tac Toe Client Close.\n");
				break;
			}
		}
	}
	close(sock);

	return 0;
}

void draw_board(GAMEBOARD* gboard) {
	char value = ' ';
	int i, j;
	printf("\n+-------------+\n");
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (gboard->board[i][j] == INIT_VALUE) {
				value = ' ';
			}
			else if (gboard->board[i][j] == S_VALUE) {
				value = 'O';
			}
			else if (gboard->board[i][j] == C_VALUE) {
				value = 'X';
			}
			else {
				value = ' ';
			}
			printf("| %c ", value);
		}
		printf("|");
		printf("\n+-------------+\n");
	}
}

int available_space(GAMEBOARD* gboard) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gboard->board[i][j] == 0) {
				return 1;
			}
		}
	}
	return 0;
}