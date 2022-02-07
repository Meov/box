#include <stdio.h>
#include "./timeslice.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

TimesilceTaskObj task_1, task_2, task_3, task_4, task_5; 

// 具体的任务函数
void task1_hdl()
{
    printf(">> task 1 is running ...\n");
}

void task2_hdl()
{
    printf(">> task 2 is running ...\n");
}

void task3_hdl()
{
    printf(">> task 3 is running ...\n");
}

void task4_hdl()
{
    printf(">> task 4 is running ...\n");
}

void task5_hdl()
{
    printf(">> task 5 is running ...\n");
}

// 初始化任务对象，并且将任务添加到时间片轮询调度中
void task_init()
{
    timeslice_task_init(&task_1, task1_hdl, 1, 1);
    timeslice_task_init(&task_2, task2_hdl, 2, 2);
    timeslice_task_init(&task_3, task3_hdl, 3, 3);
    timeslice_task_init(&task_4, task4_hdl, 4, 4);
    timeslice_task_init(&task_5, task5_hdl, 5, 5);
/*
    timeslice_task_add(&task_1);
    timeslice_task_add(&task_2);
    timeslice_task_add(&task_3);
    timeslice_task_add(&task_4);
    timeslice_task_add(&task_5);
*/
}

// 开两个线程模拟在单片机上的运行过程

void * timeslice_exec_thread(void *arg)
{
    while (1)
    {
        timeslice_exec();
    }
}

void *timeslice_tick_thread(void *arg)
{
    while (1)
    {
        timeslice_tick();
        sleep(1);
    }
}


int main(int argv, int argc[]){
    task_init();
    printf("test begin\n");
    pthread_t thread_one, thread_two;
    if( 0!=pthread_create( &thread_one, NULL, timeslice_tick_thread,NULL)){
        printf("pthread create failed! \n");
        return -1;
    }
    if( 0!=pthread_create( &thread_one, NULL, timeslice_exec_thread,NULL)){
        printf("pthread create failed! \n");
        return -1;
    }

    printf(">> task num: %d\n", timeslice_get_task_num());
    printf(">> task len: %d\n", timeslice_get_task_timeslice_len(&task_3));

    timeslice_task_del(&task_2);
    printf(">> delet task 2\n");
    printf(">> task 2 is exist: %d\n", timeslice_task_isexist(&task_2));

    printf(">> task num: %d\n", timeslice_get_task_num());

    timeslice_task_del(&task_5);
    printf(">> delet task 5\n");

    printf(">> task num: %d\n", timeslice_get_task_num());

    printf(">> task 3 is exist: %d\n", timeslice_task_isexist(&task_3));
    timeslice_task_add(&task_2);
    printf(">> add task 2\n");
    printf(">> task 2 is exist: %d\n", timeslice_task_isexist(&task_2));

    timeslice_task_add(&task_5);
    printf(">> add task 5\n");

    printf(">> task num: %d\n", timeslice_get_task_num());

    printf("\n\n========timeslice running===========\n");


    pthread_join(thread_one, NULL);
    pthread_join(thread_two,NULL);

    return 0;
}
