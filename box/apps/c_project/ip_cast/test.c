#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#if 0
typedef struct {  
    unsigned int u32_h;  
    unsigned int u32_l;  
}Int64_t;  
  
typedef union {  
    unsigned long long u64;  
    Int64_t st64;  
}Convert64_t;

//主机序转网络序
unsigned long long htonll(unsigned long long val)
{  
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
	{
		Convert64_t box_in, box_out;  
  
		box_in.u64 = val;  
		box_out.st64.u32_h = htonl(box_in.st64.u32_l);  
		box_out.st64.u32_l = htonl(box_in.st64.u32_h);  
		return box_out.u64;
	}
	else if (__BYTE_ORDER == __BIG_ENDIAN)  
    {  
        return val;
    }
}

//网络序转主机序
unsigned long long ntohll(unsigned long long val)  
{
    if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
		Convert64_t box_in, box_out;  
  
		box_in.u64 = val;  
		box_out.st64.u32_h = ntohl(box_in.st64.u32_l);  
		box_out.st64.u32_l = ntohl(box_in.st64.u32_h);  
		return box_out.u64;
	}
	else if(__BYTE_ORDER == __BIG_ENDIAN)
	{
		return val;
	}
}
#endif

void int2string(const char * ip_str){

	unsigned int ip_uint = 0;
	unsigned char *ip_p = NULL;
	inet_pton(AF_INET,ip_str,&ip_uint);
	printf("INT in_uint = %d\n",ip_uint);
	printf("HEX in_uint = %#x\n",ip_uint);
	ip_p = (char *)&ip_uint;
	printf("in_uint = %d,%d,%d,%d\n",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));
}

void portconvert(short int p){
	printf("htons(0x%04x) = 0x%04x\n", p, htons(p));
}

int main(){
	int2string("120.0.0.1");
	portconvert(6666);
}




