#include <stdio.h>
#include <stdlib.h>
#if 0
int (*p[5])() = {NULL};  // 创建一个函数指针数组
typedef int (* handler)(int param);
static handler handler_callback[5] = {NULL};


int test01(int d)  // 用户自己要添加的注册函数的函数
{
    printf("\ntest01 callback\n");

}
int test02(int a)
{
    printf("\ntest02 callback\n");
}

int test03(int n){
	printf("\ntest03 callback\n");
	printf("parameter : %d\n",n+9);
}

int register_hook_callback(int id,handler handler_function)  // 只是提供一个注册函数
{
	if(id > 5){
		return -1;
	}
	handler_callback[id] = handler_function;
}

int hook()  // 官方封装起来
{
    int i = 0;
   	for (i = 0; i < 5; i++){
		if(handler_callback[i] != NULL){
		   handler_callback[i](i);
		}

	}
}


int main(int argc, char * argv[])  // 这个一般情况时官方封装成一个库函数，上电就会一直运行，类似守护进程
{
		register_hook_callback(1,test01);
		register_hook_callback(2,test02);
		register_hook_callback(3,test03);
	        hook();
	        printf("=====运行========\n");
}

#endif

#define WORK_NUMS 10

typedef int (*pf_callback)(int a);


typedef struct WORKER{
	int worker_id;
	pf_callback worker_action;
}worker;

worker *worker_set[WORK_NUMS] = {NULL};

int worker_register(worker *p){
	int worker_num;
	if(p == NULL) return;
	while(worker_set[worker_num]){
		worker_num ++;
		printf(" num: %d\n",worker_num);
		if(worker_num > WORK_NUMS){
			printf("no space free for workers!\n");	
			free(p);
			p = NULL;
			return -1;
		}
	}
	printf("worker %d register!\n",worker_num);
	worker_set[worker_num] = p;
	return 0;
}


int worker_run(void){
	int worker_num = 0;
	printf("worker runnunununu\n");
		
	while((worker_set[worker_num])&&(worker_num < WORK_NUMS)){
		worker_set[worker_num]->worker_action(worker_set[worker_num]->worker_id);
		printf("worker %d run!\n",worker_num);
		/*free the function point*/
		free(worker_set[worker_num]);
		worker_set[worker_num] = NULL;
		worker_num ++;
	}
}



int worker_one(int a){
	printf("I am worker %d!\n",a);
	return 0;
}

int worker_two(int a){
	printf("I am worker %d!\n",a);
	return 0;
}

int main(){
	int err = -1;
	worker *worker_real;
	worker_real = (worker *)malloc(sizeof(worker));
	worker_real->worker_id = 1;
	worker_real->worker_action = worker_one;	
	worker_register(worker_real);
//	worker_run();	
	worker_real->worker_id = 2;
	worker_real->worker_action = worker_one;	
	worker_register(worker_real);
	worker_run();	
}
//#endif
