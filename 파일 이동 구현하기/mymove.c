#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define BUF_SIZE 100

int main(int argc, char* argv[])
{
	char buf1[BUF_SIZE];
	char buf2[BUF_SIZE];
	int fd1;
	int fd2;
	if (argv[1]==NULL || argv[2]==NULL)
	{
		printf("[Error] mymove Usage: ./mymove src_file dest_file\n");
	}
	else
	{
		fd1=open(argv[1], O_RDONLY); 
		fd2=open(argv[2], O_CREAT|O_RDWR|O_TRUNC, 0644);

		int size;		
		size=read(fd1, buf1, BUF_SIZE);

		strcpy(buf2, buf1);

		write(fd2, buf2, size);
 
		printf("move from %s to %s (bytes:%d) finished.\n",argv[1], argv[2], size);
		remove(argv[1]);

		close(fd1);
		close(fd2);
	}
	return 0;
}
