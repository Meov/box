#include <string.h>
#include <stdio.h>


#define MAX_SINGLE_BUF_SIZE 800     //单个节点数据容量
#define MAX_BUF_NODE_NUM 10         //总共节点数

/*
    每个buffer_node包含8k的字符空间，总共1000个buffer_node
    总共可以申请 8K*1000 = 8Mb的内存空间
*/
#define BUF_FREE 0X46524545 //'F' 'R' 'E' 'E'
#define BUF_USED 0X55534544 //'U' 'S' 'E' 'D'
#define NULLPTR (void *)0


char mem[8096];  //在堆上模拟一个8M的堆空间



//  初始化链表，指针置空
#define DLIST_INIT(list)\
do\
{\
    (list)->pNext = list;\
    (list)->pPrev = list;\
} while (0)


// 双向链表头插法
#define DLIST_ADD_BEFORE(node,newnode)\
do\
{\
    (newnode)->pNext = (node);\
    (newnode)->pPrev = (node)->pPrev;\
    ((node)->pPrev)->pNext = (newnode);\
    (node)->pPrev =  (newnode);\
} while (0)

//双向链表尾插法
#define DLIST_ADD_AFTER(node,newnode)\
do\
{\
    (newnode)->pNext = (node)->pNext;\
    (newnode)->pPrev = (node);\
    ((node)->pNext)->pPrev = (newnode);\
    (node)->pNext = (newnode);\
} while (0)



#define DLIST_FOR_EACH(node,head) for((node) = (head)->pNext;(node)!=head;(node)=(node)->pNext)
#define DLIST_IS_EMPTY(head) (head == (head)->pNext)
#define DLIST_NODE_OFFESET(type,member) ((size_t)&((type *)0)->member)  //求得的地址就是member在结构体中的偏移量
//删除node节点
#define DLIST_DEL(node)\
do\
{\
    if((node)->pPrev != NULLPTR)\
        (node)->pPrev->pNext = (node)->pNext;\
    if((node)->pNext != NULLPTR)\
        (node)->pNext->pPrev = (node)->pPrev;\
} while (0)

//链表节点
//双向链表
typedef struct DlistNode_tag
{
    struct DlistNode_tag * pNext;
    struct DlistNode_tag * pPrev;
}DlistNode;

//单向链表
typedef struct SlistNode_tag
{
    struct SlistNode_tag * pNext;
}SlistNode;

//单个buffer node结构体
typedef struct BufferNode_Tag
{
    DlistNode dlnode;
    unsigned int state;
    char buf[MAX_SINGLE_BUF_SIZE];
}BufNode;


DlistNode free_buffer_list;
//BufNode *const bffer_array = (BufNode *)BUF_ADDR
BufNode *bffer_array = (BufNode *)mem;  //模拟全部的内存  8M

/*
    将8M的内存空间，分成1000个小单元，每个单元大小为8192（8K）
    这1000个小单元的地址 组成链表的节点，每次申请的时候找空闲链表，找到后
    每次申请空间的时候，只能申请8k以下的空间
*/

void dlist_add_before(DlistNode *node,DlistNode *newnode){
    newnode->pNext = node->pNext;
    newnode->pPrev = node;
    node->pNext->pPrev = newnode;   //将其与node节点的后继节点链接起来
    node->pNext = newnode;
}

void dlist_add_after(DlistNode *node,DlistNode *newnode){
    newnode->pNext = node->pNext;
    newnode->pPrev = node;
    node->pNext->pPrev = newnode;
    node->pNext = newnode;

}
int buffer_init(void){
    int i;
    //初始化数据块 8Mb的数据块
    memset((void *)bffer_array,0,sizeof(*bffer_array));
    //初始资源列表，存放每二个数据单元的地址
    printf("enter :%s\n",__func__);
	DLIST_INIT(&free_buffer_list);
    //初始化空闲列表
    printf("BufNode size: %lu sizeof(*bffer_array) %lu\n",sizeof(BufNode),sizeof(mem));
    for(i = 0; i<MAX_BUF_NODE_NUM;i++){  //地址往后偏移
	bffer_array[i].state = BUF_FREE;
      //  printf("free_buffer_list addr : %p,bffer_array.dlnode : %p\n",&free_buffer_list,&(bffer_array[i]));
        dlist_add_before(&free_buffer_list, &(bffer_array[i].dlnode)); //将数据节点以头插法的形式插入列表中
    }
    return 0;
}

void* buffer_alloc(unsigned int size){
    static int  alloc_num = 0;
    alloc_num++;
    DlistNode *dlnode = NULLPTR;
    BufNode *buffer_node_ptr = NULLPTR;
    
    if(DLIST_IS_EMPTY(&free_buffer_list)){
        printf("free_buffer_list is empty!\n");
        return NULLPTR;
    }
    if(size > MAX_SINGLE_BUF_SIZE){
        printf("you can only olloc bytesize less than %d\n",MAX_SINGLE_BUF_SIZE);
        return NULLPTR;
    }
    DLIST_FOR_EACH(dlnode,&free_buffer_list){
        buffer_node_ptr = (BufNode *)((char *)dlnode - DLIST_NODE_OFFESET(BufNode,dlnode));
        //因为要重新将链表强行转换成 BufNode 类型，所以要将地址往回偏移 dlnode 地址
        //值得好好分析！    
        if(buffer_node_ptr->state == BUF_FREE){
            break;
        }
        else
        {
            printf("no free space in free list\n");
            return NULLPTR;
        }   
    }
    //发生了偏移 说明遍历成功
    if(dlnode != &free_buffer_list){
        DLIST_DEL(&(buffer_node_ptr->dlnode)); //删除buffer中的节点
        buffer_node_ptr->state = BUF_USED;
        printf("++++++++++++++++setted addr :%p\n",buffer_node_ptr);
    }
    else
    {
        printf("list didnt move! no free space in free list\n");
        return NULLPTR;
    }
    return (void *)(buffer_node_ptr);
}

void buffer_free(void * buf){
    DlistNode *dlnode = NULLPTR;
    BufNode *buffer_node_ptr = NULLPTR;

    if(buf == NULLPTR){
        return;
    }
    buffer_node_ptr = (BufNode *)((char*)buf - DLIST_NODE_OFFESET(BufNode,buf));
    if(buffer_node_ptr->state != BUF_USED){
        
        printf("----------------------free addr :0x%c\n",buffer_node_ptr->state); 
        printf("err! buf is not used!\n");
        return;
    }
    buffer_node_ptr->state = BUF_FREE;
    //加入空闲资源链表 使用尾插法
    dlist_add_after(&free_buffer_list,&(buffer_node_ptr->dlnode));
}

int main(int argv, char **argc){
    char* ptr_test[10];
    BufNode *buffer_node_ptr[10]; 
    char *malloced_mem; 
    int i = 0;
    buffer_init();
    printf("init end!\n");
#if 1
//    for(i = 0; i<12; i++){
        malloced_mem = (char*)buffer_alloc(800);
 	if(malloced_mem != NULL)
	printf("num %d addr :0x%p \n",i,malloced_mem); 
	

       	//buffer_node_ptr = ( BufNode *)(ptr_test[i]);
        //printf("state: = 0x%x\n",buffer_node_ptr[i]->state);
        //if(i % 2 == 0 ){
           // printf("\n");
           // buffer_free(buffer_node_ptr[i]);
        //}     
 //   }
	char buf = 0x12;
	for(i = 0; i<8096; i++){
		malloced_mem[i] = buf;
	}

	
	buffer_free(malloced_mem);
#endif
}
