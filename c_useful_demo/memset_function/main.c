#include <stdio.h>
#include<string.h>

int main(int argc, char *argv[]){

	int a[10];
	int i = 0;

	memset(a,1,sizeof(a));

	printf("sizeof a: %ld\n",sizeof(a));
	
	for(i = 0; i < 10; i++){
		printf("a[%d]: [hex]0x%x  [dec]%d\n",i,a[i],a[i]);
	}

	return 0;
}
