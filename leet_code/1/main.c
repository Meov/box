/*
 * @lc app=leetcode.cn id=1 lang=c
 *
 * [1] 两数之和
 */

// @lc code=start


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLENGTH 1000
#define DEFUALT_NULL_VALUE -1
#define PROPERTY 2

struct hash{
	int * data;
	int length;
};

int init_hash(struct hash *H){
	H->length = MAXLENGTH;
	H->data = (int *)malloc(sizeof(int)*MAXLENGTH);
	if(H->data == NULL){
		return -1;
	}
	memset(H->data,DEFUALT_NULL_VALUE,sizeof(int)*MAXLENGTH); //利用了memset的特性，一般赋值为0 -1是巧合，其他值不可为非char初始化
	return 0;
}

int hash_trans(int key){
	
	return key%PROPERTY;

}

int find_hash(struct hash *H, int value){
	int addr = 0;
	
	while(H->data[addr]!=value){
		addr ++;
		
		if(addr >= MAXLENGTH){
			return 0; //no data in hash
		}
	}
	
	return addr; //find data
}

int insert_hash(struct hash *H, int value){
	int addr = 0;
	addr = find_hash(H,value);

	if(addr){
		printf("value %d  found!\n",value);
		return -1;
	}
	addr ++;  //store it next position
	if(addr <= MAXLENGTH)
		H->data[addr] = value;
	return 0;
}


int* twoSum(int* nums, int numsSize, int target, int* returnSize){
   // int result_arr[2];
    int i = 0;
    int temp = 0;
    int *a = (int*)malloc(sizeof(int)*numsSize);
    int *result_arr = (int *)malloc(sizeof(int)*2);
   
    memset(result_arr,0,sizeof(a)); 
    memset(a,-1,sizeof(a));
    *returnSize = 0;
    for(i = 0; i < numsSize ;i++){   //creat a hash map
        a[nums[i]] = i;
    }
    for(i = 0; i < numsSize; i++){
        temp = target - nums[i];
	if((a[temp] != -1)&&(i != a[temp])){  //caculate wether this is a result stored in the hash map
		result_arr[0] = a[temp];
		result_arr[1] = i;
		printf("%d ,%d\n",result_arr[0],result_arr[1]);
		*returnSize = 2;
		break; 
	}
    }
    free(a);
    a = NULL;
    return result_arr;
}

int main(){
    int num = 0;
    int a[7] = {1,1,3,4,5,6,7};
    int target = 8;
    twoSum(a,sizeof(a)/sizeof(int),target,&num);
}
// @lc code=end


