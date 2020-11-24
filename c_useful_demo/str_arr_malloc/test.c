#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char ** chat_grp;
int main(int argc,char **argv){
	int i;
	int string_num;
	int str_len;
	
	string_num = argc;
	printf("grp number is %d\n",string_num);
	chat_grp = (char **)malloc(sizeof(char *)*string_num);
	for (i = 0; i < string_num; i++){
		if(argv[i] == NULL) break;
		str_len = strlen(argv[i]);
		printf("str_len:%d\n",str_len);		
		chat_grp[i] = (char *)malloc(sizeof(char) * str_len);
		if(chat_grp[i] == NULL) printf("malloc err\n");	
		else {
			printf("chat_grp[%d]: addr : %p allocated!\n",i,chat_grp[i]);
			strcpy(chat_grp[i],argv[i]);
			printf("chat_grp %d is %s\n",i,chat_grp[i]);
		}
	}
	/*release the memory allocated*/
	for(i = string_num-1; i >= 0; i--){
		printf("chat_grp[%d]: addr : %p freed!\n",i,chat_grp[i]);
		free(chat_grp[i]);
		chat_grp[i] = NULL;
	}
	free(chat_grp);
	chat_grp = NULL;
}
