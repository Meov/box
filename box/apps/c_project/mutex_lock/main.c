#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#if 1
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
void * thread_func_one(void *arg)
{
   int i;
   for(i=0;i<5;i++){
//    pthread_mutex_lock( &mutex1);
//     pthread_mutex_lock( &mutex1);//锁两次
     count++;
     sleep(1);
//     pthread_mutex_unlock(&mutex1);
//     pthread_mutex_unlock(&mutex1);
     printf("thread one count value is %d\n",count);
   }
   printf("thread one over\n");
   return NULL;
}
void * thread_func_two(void *arg)
{
   int i;
   for(i=0;i<5;i++){
//      pthread_mutex_lock( &mutex1);
//	pthread_mutex_lock( &mutex1);
     count++;
     sleep(1);
//     pthread_mutex_unlock(&mutex1);
//     pthread_mutex_unlock(&mutex1);
     printf("thread two count value is %d\n",count);
   }
 
   printf("thread two over\n");
   return NULL;
}

#else

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
void * thread_func_one(void *arg)
{
   int i;
   for(i=0;i<5;i++){
     pthread_mutex_lock( &mutex1);
     count++;
     sleep(1);
     pthread_mutex_unlock(&mutex1);
     printf("thread one count value is %d \n",count);
   }
   return NULL;
}
void * thread_func_two(void *arg)
{
   int i;
   for(i=0;i<5;i++){
     pthread_mutex_lock( &mutex1);
     count++;
     sleep(1);
     pthread_mutex_unlock(&mutex1);
     printf("thread two count value is %d \n",count);
   }
   return NULL;
}

#endif
int main ( int argc, char **argv)
{
   printf("test begin\n");
   pthread_t thread_one, thread_two;
   if( 0!=pthread_create( &thread_one, NULL, thread_func_one,NULL)){
      printf("pthread create failed! \n");
      return -1;
   }
   if( 0!=pthread_create( &thread_one, NULL, thread_func_two,NULL)){
      printf("pthread create failed! \n");
      return -1;
   }
   pthread_join(thread_one, NULL);
   pthread_join(thread_two,NULL);
   return 0;
}



