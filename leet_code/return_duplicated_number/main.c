#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int return_duplicated_number(int arr[],int lenth){
	int b_arr[lenth];
	int i = 0;
	memset(b_arr,0,sizeof(b_arr));

	if((arr == NULL)||(lenth <= 1)){
		printf("err dataset!\n");
	}

	for(i=0; i<lenth; i++){
		if(b_arr[arr[i]] == 0)		//create hash table 
			b_arr[arr[i]] = 1;
		else
			return  arr[i];
	}

	printf("not found!\n");
	return -1;
}

int main(int argc, char *argv[]){

	int a = 11;
	int *data;
	int b[40];
	int test[6] = {1,2,3,0,5,1};

	data = (int *)malloc(sizeof(int)*100);
	printf("a: %d b :%ld sizeof(int):%ld sizeof(*int): %ld\n",a,sizeof(b),sizeof(data),sizeof(*data));
	
	printf("duplicated number is %d\n",return_duplicated_number(test,sizeof(test)/sizeof(int)));
		
}
