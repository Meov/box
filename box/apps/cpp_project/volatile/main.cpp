/* Compile code with optimization option */
#include <stdio.h> 

int main(void) 
{ 
    //const volatile int local = 10;    //想想一个只读的状态标志位的值，应该加上volatile防止每次查询的时候都去查找寄存器中备份的值。
    const int local = 10;      //未加上volatile关键字 以下是一个例子
    int *ptr = (int*) &local; 

    printf("Initial value of local : %d \n", local); 

    *ptr = 100; 

    printf("未使用volatile修饰这个变量");
    printf("Modified value of local  : %d  并没有发生变化，跟初值一样\n", local);     //修改了local这个变量地址处的值，程序本意是修改local的值 但是并非与预期中的一致。 
    printf("Modified local addr value: %d 该值对应的内存地址中的内容已经被修改了\n", *ptr); 
    printf("local addr: %p  ptr addr %p 可以发现地址也一样\n", &local, ptr);  //这两个地址是一样的
    return 0; 
} 
