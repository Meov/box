本题采用哈希表的做法最为简单
在main.c中简单实现了下功能，原理如代码中写到的一样
1. 先将所有的数据存放进哈希表里，作为索引，其存放的位置作为哈希表的值
2. 在下一轮计算时，计算得到的值是否在哈希表中，若存在则可以直接获得地址


看到一种改进的做法，设计哈希映射函数，可以参考一下；

#define MAX 2000
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int *ret=(int *)malloc(sizeof(int) * 2);
    int hash[MAX];
    memset(hash,-1,sizeof(hash));

    for(int i = 0; i < numsSize; i++) {
        if (hash[(target - nums[i] + MAX) % MAX] != -1) {
            ret[0] = hash[(target - nums[i] + MAX) % MAX];
            ret[1] = i;
            *returnSize = 2;
            return ret;
        }
        hash[(nums[i] + MAX) % MAX] = i;
    }
    free(hash);
    *returnSize = 0;
    return ret;
}

但是这两种做法有一个巨大的问题，若数据中存在负数，负数作为数组索引值会引发错误，需要继续设计哈希函数

