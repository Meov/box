#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long long ipstr2int(char * ip_str){
	int i,j;
	int tmp;
	unsigned long long ip_num = 0;
	char *ipstr = ip_str;
	char tmp_str[3];
	int bit_dot = 3;
	int str_len = strlen(ip_str);  //不包含字符串结束符的
	j = 0;
	if((str_len > 15)||(ipstr == NULL)) return 0;

	for(i = 0; i <= str_len;i++){
		//192.168.60.199
		if((ipstr[i] == '.')||(ipstr[i] == '\0')){  //以"."分割字符串，以及结尾之前的字符串
			ip_num += atoi(tmp_str)  <<  (bit_dot * 8);
			printf("num: %d bit_dot: %d ip_num: %llu\n",atoi(tmp_str),bit_dot,ip_num);
			bit_dot --;
			memset(tmp_str,0,sizeof(tmp_str));
			j = 0;
		}else{
			tmp_str[j++] = ipstr[i];
		}
	}
	return ip_num;
}

int main(int argc, char* argv[]){
	unsigned long long res = 0;
	char *ip = "192.168.62.199";
	res = ipstr2int(ip); 
	printf("ip str: %s  ip long: %llu\n",ip,res);
	return 0;
}
