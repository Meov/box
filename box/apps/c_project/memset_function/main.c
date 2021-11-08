
#include <string.h>
#include <stdio.h>
#define MAXSIZE 100000
 
int main()
{
	int i;
	int j;
    char arr[MAXSIZE];
    for(i=0;i<100000;i++)
    {
#if 0
	    memset(arr,'1',sizeof(arr));
#else
	    for(j=0;j<MAXSIZE;j++)
		    arr[j] = '1';
#endif
    }


	for(i = 0 ; i < MAXSIZE; i++){
		printf("%c",arr[i]);

	}

    return 0;
}
