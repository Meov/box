/*
 * @lc app=leetcode.cn id=3 lang=c
 *
 * 给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。
 */

// @lc code=start
#include <stdio.h>
#include <string.h>
#if 0
int searchInsert(int* nums, int numsSize, int target){
	int i = 0;
	for(i = 0; i<numsSize; i++){
		if(nums[i] == target){
		return i;
		}
	}
	return -1;
}
#endif
int searchInsert(int* nums, int numsSize, int target){
	int middle = 0;
	int left = 0;
	int right;
	if(target > nums[numsSize-1])
		return numsSize;
	right = numsSize - 1;
	while(left <= right){
		if(nums[middle] < target){
			left = middle + 1;
		}else if(nums[middle] > target){
			right = middle - 1;
		}else{
			return middle;
		}
		middle = (right - left)/2 + left;
	}
	return middle;
}
int main(int argc ,int *argv[]){
	int array_1[10] = {1,2,3,4,5,6,7,8,9,0}; 
	printf("result: %d\n", searchInsert(array_1,10,6));
	int array_2[11] = {1,2,3,4,5,6,7,8,9,10,11}; 
	printf("result: %d\n", searchInsert(array_2,11,6));
	int array_3[2] = {1,2}; 
	printf("result: %d\n", searchInsert(array_3,2,6));
}
// @lc code=end

