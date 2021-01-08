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
    struct ListNode *list1 = l1;
    struct ListNode *list2 = l2;
    unsigned int sum = 0;
    unsigned int sum_inc = 0; //进位位
    unsigned int list1_val = 0;
    unsigned int list2_val = 0;
    struct ListNode *list_sum = (struct ListNode*)malloc(sizeof(struct ListNode));
    list_sum->next = NULL;
    struct ListNode *list_sum_tem = list_sum;
    struct ListNode *list_temp;

    if(list1 == NULL && list2 == NULL){
        printf("lists entered are all NULL\n");
        return NULL;
    }
    while (list1 || list2)
    {
        list_temp = (struct ListNode *)malloc(sizeof(struct ListNode));
        list_temp->next = NULL;

        if(list1){
            list1_val = list1->val;
            list1 = list1->next;
        }else{
            list1_val = 0; 
        }   
        if(list2){
            list2_val = list2->val;
            list2 = list2->next;   
        }else{
            list2_val = 0;
        }
        sum = list1_val + list2_val;
        printf("----sum: %d sum_inc: %d\n",sum,sum_inc);
        sum = sum + sum_inc;
        //头插法扩展链表
        
        if(sum >= 10){ //有进位位
            sum_inc = 1; 
            list_temp->val = sum % 10;
        }else{         //无进位位
            list_temp->val = sum;
            sum_inc = 0;
        }

        list_sum_tem->next = list_temp;
        list_sum_tem = list_temp;
    }
    if(sum_inc){
        list_temp = (struct ListNode *)malloc(sizeof(struct ListNode));
        list_temp->next = NULL;
        list_temp->val = sum_inc;
        list_sum_tem->next = list_temp;
        list_sum_tem = list_temp;
    }
    return list_sum->next;
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

