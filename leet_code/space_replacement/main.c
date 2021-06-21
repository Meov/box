#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//将空格替换成 %20 字符
int main(int argc, char *argv[]){
	char string[100]  = "we are the world";
 	char *p = string;
	int str_len = 0;
	int space_num = 0;
	int new_str = 0;
	//str_len = strlen(string);
	while(*p != '\0'){
		str_len++;
		printf("%c \n",*p);
		if(*p++ == ' '){
			space_num++;
		}
	}

	printf("str_lrn: %d  space num: %d\n",str_len,space_num);
	
	new_str = str_len + space_num*2;  /* ' ' -> %20 */
	int i = 0;
	for(i = str_len; i >= 0; i--){
		if(string[i] == ' '){
			string[new_str--]  = '0';
			string[new_str--]  = '2';
			string[new_str--]  = '%';
		}else{
			string[new_str--]  = string[i];
		//	printf("string[%d]: %c  string[%d]: %c \n",new_str,string[new_str],i,string[i]);
		}	
	}

	printf("%s \n",string);

	return 0;

}
