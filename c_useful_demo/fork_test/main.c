#include <stdio.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>
typedef __pid_t pid_t;
int j = 88;

int main(){
	pid_t pid;
	int i = 10;
	
	i--;	
//	for(i=0; i<3; i++)
		pid = fork();
	
	if(pid == 0){
		i++;
		j+=2;
		printf("---------------child i: %d j: %d\n",i,j);
		exit(0);
	}
	printf("-----------------parent i %d j %d\n",i,j);
	exit(0);
}
