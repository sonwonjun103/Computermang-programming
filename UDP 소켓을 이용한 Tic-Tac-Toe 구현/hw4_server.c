//2017113724 손원준
//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

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
	int serv_sock;
	socklen_t clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;
	int row, column;

	GAMEBOARD s;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	for(int i=0;i<BOARD_SIZE;i++){
		for(int j=0;j<BOARD_SIZE;j++){
			s.board[i][j]=0;
		}
	}

	printf("Tic-Tac-Toe Server\n");

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0); //UDP(소켓 생성) - SOCK_DGRAM

	if (serv_sock == -1) {
		printf("UDP socket creation error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		printf("bind() error");
	}

	srand(time(NULL));
	draw_board(&s);
	int cnt=0;

	while (1){
		
		row = rand() % BOARD_SIZE;
		column = rand() % BOARD_SIZE;
		
		//만약에 s.board[row][column]이 0이 아닐시 다시 클라이언트로부터 받으면 안되므로 recvfrom에 조건 설정
		if(cnt==0){
			clnt_adr_sz = sizeof(clnt_adr);
			recvfrom(serv_sock, s.board, sizeof(GAMEBOARD), 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
			draw_board(&s);
			//서버로부터 한번만 받기
			cnt=1;
		}

		if (row < BOARD_SIZE && column < BOARD_SIZE && s.board[row][column] == 0) {
			printf("Server choose : [%d %d]\n", row, column);
			s.board[row][column] = S_VALUE;
			sendto(serv_sock, s.board, sizeof(GAMEBOARD), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
			draw_board(&s);
			//보내기 성공시 cnt 초기화
			cnt=0;
		}
		if (available_space(&s) == 0) {
			printf("No available space. Exit this program.\n");
			printf("Tic Tac Toe Server Close.\n");
			break;
		}
		
	}
	close(serv_sock);
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

int available_space(GAMEBOARD* gboard){
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gboard->board[i][j] == 0) {
				return 1;
			}
		}
	}
	return 0;
}
