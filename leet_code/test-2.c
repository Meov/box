/*
 * @lc app=leetcode.cn id=2 lang=c
 *
 * [2] 两数相加
 */

// @lc code=start
/**
 * Definition for singly-linked list.*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct ListNode {
     unsigned int val;
     struct ListNode *next;
 };
 


struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    unsigned int sum = 0;
    struct ListNode* list_sum = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(!list_sum) return NULL;
    list_sum->next = NULL;
    int list_lenth = 0;
    unsigned int num1 = 0;
    unsigned int num2 = 0; 
    struct ListNode* list1 = l1;
    struct ListNode* list2 = l2;
    printf("num1: %d,num2: %d\n",num1,num2);
    struct ListNode* list_sum_final;
    list_sum->next = NULL;
    list_sum_final = list_sum; 
    //获取链表长度
    while ((list1->next)||(list2->next))
    {
       
	int Proportion =  (int)pow(10,list_lenth);
	
	num1 += (list1->val)*Proportion;        
	num2 += (list2->val)*Proportion;
    	printf("num1: %d num 2:%d\n",num1, num2);
	list_lenth++;
        list1 = list1->next; 
        list2 = list2->next;
    } 
	sum = num1+num2;
    while (list_lenth--)
    {
        struct ListNode* list_sum_tem = 
        (struct ListNode*)malloc(sizeof(struct ListNode));
    	    
	list_sum_tem->val = sum / (unsigned int)(pow(10,list_lenth));
    	
	//list_sum_tem->val = list_lenth;
    	
	printf("val: %d\n",list_sum_tem->val);
	
	list_sum_tem->next = list_sum->next;
        list_sum->next = list_sum_tem;
    }
    //list_sum->next = NULL;
    list_sum_final = list_sum->next;
    while(list_sum_final){
	printf("----val: %d\n",list_sum_final->val);
	list_sum_final = list_sum_final->next; 
    }

    printf("answer is :%d\n",sum);
  //  return list_sum->next;

}


void LinkListCreate(const int n, struct ListNode *head)
{
    int i = 0;
    struct ListNode *rear;
    struct ListNode *p;
//    struct ListNode * head = (struct ListNode*)malloc(sizeof(struct ListNode));
    rear = head;
    head->val = 0;
    for(i=0;i<n;i++)
    {
        p = (struct  ListNode*)malloc(sizeof(struct ListNode));
	p->val = i+1;
        rear->next = p; //尾指针 指向新建结点
        rear = p;//rear指针移动
    }
    rear->next = NULL;

}

int main(int argc ,int *argv[]){
	int i =0;
	struct ListNode* list1 = (struct ListNode*)malloc(sizeof(struct ListNode));
	struct ListNode* list2 = (struct ListNode*)malloc(sizeof(struct ListNode));
	LinkListCreate(5, list1);
	LinkListCreate(5, list2);
	/*
	while(list1){
		printf("list1 val: %d\n",list1->val);
		list1 = list1->next;
	}
	*/
	addTwoNumbers(list1,list2);
}
// @lc code=end

