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
#define TEST2

void sigint_handler_dealchild(int sig){

#if 1
	/* this will create defunc progress, cause the third sigal has been discarded.  reference 《深入理解计算机系统》p.537*/
	/*
		first SIGCHLD signal  will be caught and executed
		second SIGCHLD signal will in pending state and will be caught forcely by this function
		third sigal will be discarded
	*/

	if(waitpid(-1,NULL,0)<0){
		printf("Handler reaped child err\n");
	}	
		printf("Handler reaped child \n");
#else
	/*
		reaped child progress as possible	
		this way is the proper method to reaped child progresses.
		只要有一个SIGCHLD 被捕捉便会一直等待回收子进程
	*/
	while(waitpid(-1,NULL,0)>0){              
		printf("Handler reaped child\n");
	}
#endif
	sleep(1);
}

void sigint_handler(int sig){
	printf("Caught SIGINT!\n");
	exit(0);
}


int main(int argc, char *argv[]){
	int i;
	pid_t pid;

#ifdef TEST1
	if(signal(SIGINT, sigint_handler) == SIG_ERR){
		printf("siganl err\n");
	}
	pause();

#endif


#ifdef TEST2
	if(signal(SIGCHLD, sigint_handler_dealchild) == SIG_ERR){
		printf("siganl err\n");
	}

	for(i = 0; i <3; i++){
		pid = fork();
		if(pid == 0){
			printf("from child :%d\n",(int)getpid());
			exit(0);
		}
	}

	while(1){
		//hang

	}

#endif

#ifdef TEST2
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
#endif
	return 0;
}
