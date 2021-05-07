#include <stdio.h>

extern int sum(int a, int b);
int main(int argc, char *argv[]){
	printf("linker test begin! refer book page467 \n");

	int var = sum(1,2);
	printf("var is %d\n",var);

}
