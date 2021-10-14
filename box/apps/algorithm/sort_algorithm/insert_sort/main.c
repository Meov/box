#include <stdio.h>
#include <string.h>

int insert_sort(int arr[], int lenth){
	int i,j;
	int tmp;
	for(i = 1; i < lenth; i++){
		for(j = i; j > 0; j--){
			if(arr[j] < arr[j-1]){  //核心代码 与之前的比较
				tmp = arr[j];
				arr[j] = arr[j-1];
				arr[j-1] = tmp;
			}
		}
	}
}

int main(int argc, char* argv[]){
	int nums[5] = {1,3,4,2,0};
	int i = 0;
	int arr_len = sizeof(nums)/sizeof(nums[0]);
	insert_sort(nums,arr_len);
	for(i = 0; i < arr_len; i++){
		printf("%d ,", nums[i]);
	}
	printf("\n");
	return 0;
}
