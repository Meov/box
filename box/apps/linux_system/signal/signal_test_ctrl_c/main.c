#include <stdio.h>
#include <bits/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
void fun_sig1(int sig);

void fun_sig2(int sig){

	printf("sig_2 = %d\n",sig);
	signal(sig,SIG_DFL);	//在这里将该信号值 SIGINT 调用的回调函数改变了。所以会调用默认程序
				//因此在第三次按下 ctrl+c 之后会调用默认函数将进程终止掉
}

void fun_sig1(int sig){
	printf("sig = %d\n",sig);
	signal(sig,fun_sig2);	//第二次收到信号时，执行默认值？？？
}



int main(int argc, char *argv[]){

	signal(SIGINT,fun_sig1);
	//signal(sig,SIG_DFL);  //执行默认值
	//signal(sig,SIG_IGN);	//忽略该信号
	while(1){
		printf("running...\n");
		sleep(1);
	}

	exit(0);
}
