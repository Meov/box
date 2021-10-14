#include <stdio.h>
void fun1(int data);
void fun2(int data);
void fun3(int data);

int g_cunt;

void fun1(int data){
	printf("%s %d called\n",__func__,__LINE__);
	g_cunt ++;
	int data_1 = data;
	fun2(data_1);
}
void fun2(int data){
	printf("%s %d called\n",__func__,__LINE__);
	g_cunt ++;
	int data_2 = data;
	fun3(data_2);
}

void fun3(int data){
	int num = data;
	int *p  = 0;
	g_cunt ++;
	printf("%s %d called\n",__func__,__LINE__);
	*p = num; //assigned data to 0 pointer
}


int main(int argc, char *argv[]){
	printf("Hi test\n");
	g_cunt ++;
	fun1(123);
}

