#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ListNode{
	int value;
	struct ListNode *next;
};

struct ListNode* create_list(int node_num){
	if(node_num <= 0)	return NULL;
	int i = 0;
	struct ListNode *Head_node = (struct ListNode *)malloc(sizeof(struct ListNode));
	struct ListNode * HEAD = Head_node;
	struct ListNode * pnode;
	Head_node->value = 0;
	Head_node->next = NULL;
	for(i = 0; i < node_num; i++){ //尾插法创建链表
		pnode = (struct ListNode *)malloc(sizeof(struct ListNode));
		Head_node->next = pnode;
		pnode->value = i+1;
		Head_node = pnode;
	}
	Head_node->next = NULL;
	return HEAD;
}

/*采用递归的方法打印 
 *想象递归的调用关系，每次访问节点之后的数据。
  想象调用的时候，将递归函数print_linstnode_revesely替换成函数实体
  这将函数自己调用自己，最后也会有一层层的打印语句
 *
 * */
void print_linstnode_revesely(struct ListNode * Head){
	if((Head->next != NULL)&&(Head != NULL)){
		print_linstnode_revesely(Head->next);
	}
	printf("%d\n",Head->value);
}

int main(int argc, char* argv[]){
	struct ListNode* Head;
	Head = create_list(78);
#if 0
	while(Head->next != NULL){
		printf("value: %d\n",Head->value);
		Head = Head->next;
	}
#endif
	print_linstnode_revesely(Head);

	printf("\n");
	return 0;
}
