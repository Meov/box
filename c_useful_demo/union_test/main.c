#include <stdio.h>


int main(int argc, char *argv[]){

	union {
		unsigned int data[2];
		unsigned int d_data;
	}temp;

	temp.data[0] = 0x1234;
	temp.data[1] = 0x5678;
	temp.d_data = 0xffff;

	printf("d_data: 0x%x addr: %p\n",temp.d_data,&temp.d_data);
	printf("data: 0x%x addr: %p\n",temp.data[0],&temp.data[0]);

	union w
	{
		int a;
		char b;
	}c;

	c.a = 1;

	printf("c.b = %d\n",c.b);// 1 little endian 0 big endien


}
