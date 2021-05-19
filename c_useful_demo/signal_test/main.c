#include <stdio.h>
#include <stdio.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]){
	pid_t pid;

	pid = fork();
	if(pid == 0){
		printf("I am running as child!\n");
		pause();
		printf("control should never reach here!\n");
		exit(0);
	}

	sleep(3);
	kill(pid, SIGKILL);
	exit(0);

}
