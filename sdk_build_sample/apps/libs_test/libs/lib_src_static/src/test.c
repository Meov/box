#include <stdio.h>
#include <statictest.h>

void static_test(void){
	printf("i am from a static lib : libtest.a\n");
	shared_test();
}
