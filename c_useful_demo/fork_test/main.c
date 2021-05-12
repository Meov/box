#include <stdio.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef __pid_t pid_t;
int j = 88;
char *buf = "abc";
char *buf_1 = "123";


int main(){
	pid_t pid;
	int fd;
	int i = 10;
	i--;	
	fd = open("test.txt",O_RDWR|O_CREAT|O_EXCL,S_IRWXU);
	//fd = open("test.txt",O_RDWR|O_CREAT);
	write(fd,buf,strlen(buf));
	pid = fork();	
	pid = fork();	
	pid = fork();	
	pid = fork();	
	if(pid == 0){
		i++;
		j+=2;
		write(fd,buf_1,strlen(buf));
		printf("--------pid :%d-------child i: %d j: %d\n",getpid(),i,j);
		exit(0);
	}
	close(fd);
	printf("-----------------parent i %d j %d\n",i,j);
	exit(0);
}
