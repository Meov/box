#include <stdio.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>
typedef __pid_t pid_t;

int main(){
	pid_t pid;
	int i = 10;
	
	for(i=0; i<2; i++)
		pid = fork();
	
	if(pid == 0){
		printf("---------------child\n");
		exit(0);
	}
	
	sleep(2);
	printf("-----------------parent \n");
	exit(0);
}
