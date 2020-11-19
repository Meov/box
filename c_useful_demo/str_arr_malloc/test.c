#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char ** gpio_grp;
int main(int argc,char **argv){
	int i;
	int grp_num;
	int str_len;
	
	grp_num = argc;
	printf("grp number is %d\n",grp_num);
	gpio_grp = (char **)malloc(sizeof(char *)*grp_num);
//	printf("argv: %s\n",argv[1]);
	for (i = 0; i < grp_num; i++){
		
		if(argv[i] == NULL) break;
		str_len = strlen(argv[i]);
		printf("str_len:%d\n",str_len);		
		gpio_grp[i] = (char *)malloc(sizeof(char) * str_len);
		if(gpio_grp[i] == NULL) printf("malloc err\n");	
		else {
			printf("dada\n");
			strcpy(gpio_grp[i],argv[i]);
			printf("gpio_grp %d is %s\n",i,gpio_grp[i]);
		}
	}
	printf("gpio_grp addr : %p  is :%s\n",gpio_grp,gpio_grp[0]);
	printf("gpio_grp + 1 addr : %p size is :%s\n",gpio_grp+1,gpio_grp[1]);
	free(gpio_grp);
	gpio_grp = NULL;
	printf("gpio_grp addr %p is %s\n",gpio_grp[1],*(gpio_grp+1));

}
