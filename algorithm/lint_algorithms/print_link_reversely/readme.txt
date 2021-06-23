1. 主要是尾插法创建链表
2. 递归的方法，从尾到头打印链表中的数。
	递归的理解，假设链表的长度为2，即只有两个节点，将递归展开如下:

	Head 指向一个含有两个节点的链表：

void print_linstnode_revesely(struct ListNode * Head){
        if((Head->next != NULL)&&(Head != NULL)){
                print_linstnode_revesely(Head->next);
        }
        printf("%d\n",Head->value);
}

展开后将会是如下情况：
void print_linstnode_revesely(struct ListNode * Head){
        if((Head->next != NULL)&&(Head != NULL)){
                //print_linstnode_revesely(Head->next);
		if((Head->next != NULL)&&(Head != NULL)){
			//print_linstnode_revesely(Head->next); 
			//两个节点时，此时判断next为NULL,不会往下递归, 依次从里往外执行打印函数。
		}
		printf("%d\n",Head->value);  //打印第二个节点

        }
        printf("%d\n",Head->value);	//打印第一个节点
}
